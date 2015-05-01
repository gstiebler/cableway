// (C) Copyright 2014 by  
//
using System;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.EditorInput;

using Newtonsoft.Json;
using System.Collections;
using System.IO;
using Newtonsoft.Json.Linq;

// This line is not mandatory, but improves loading performances
[assembly: CommandClass(typeof(AutoCAD_CSharp_plug_in1.MyCommands))]


namespace AutoCAD_CSharp_plug_in1
{

    // This class is instantiated by AutoCAD for each document when
    // a command is called by the user the first time in the context
    // of a given document. In other words, non static data in this class
    // is implicitly per-document!
    public class MyCommands
    {

        public static cwConversor.BaseObj getCWObject(DBObject dbo, int id)
        {
            cwConversor.BaseObj obj = null;
            if (dbo is Circle)
                obj = new cwConversor.Circle(dbo as Circle, id);
            else if (dbo is Arc)
                obj = new cwConversor.Arc(dbo as Arc, id);
            else if (dbo is Line)
                obj = new cwConversor.Line(dbo as Line, id);
            else if (dbo is MText)
                obj = new cwConversor.MText(dbo as MText, id);
            else if (dbo is DBText)
                obj = new cwConversor.DBText(dbo as DBText, id);
            else if (dbo is Polyline)
                obj = new cwConversor.Polyline(dbo as Polyline, id);

            return obj;
        }


        public static string DrawingPath()
        {
            Document doc = Application.DocumentManager.MdiActiveDocument;
            HostApplicationServices hs = HostApplicationServices.Current;
            string path = hs.FindFile( doc.Name, doc.Database, FindFileHint.Default );
            return path;
        }

        
        [CommandMethod("dwg2json")]
        public static void dwg2json()
        {
            cwConversor.JsonDwg jsonDwg = new cwConversor.JsonDwg();

            // Get the current document and database, and start a transaction
            Document acDoc = Application.DocumentManager.MdiActiveDocument;
            string outputFileName;
            {
                string path = DrawingPath();
                int dotIndex = path.LastIndexOf('.');
                string pathAndFileName = path.Substring(0, dotIndex);
                outputFileName = pathAndFileName + ".json";
            }
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(@outputFileName))
            {
                Database acCurDb = acDoc.Database;

                using (Transaction acTrans = acCurDb.TransactionManager.StartTransaction())
                {
                    // Open the Block table record for read
                    BlockTable acBlkTbl;
                    acBlkTbl = acTrans.GetObject(acCurDb.BlockTableId,
                                                 OpenMode.ForRead) as BlockTable;

                    // Open the Block table record Model space for read
                    BlockTableRecord acBlkTblRec;
                    acBlkTblRec = acTrans.GetObject(acBlkTbl[BlockTableRecord.ModelSpace],
                                                    OpenMode.ForRead) as BlockTableRecord;

                    DBDictionary groups = acTrans.GetObject(acCurDb.GroupDictionaryId, OpenMode.ForRead) as DBDictionary;

                    int id = 1;
                    System.Collections.Generic.HashSet<DBObject> usedObjects = new System.Collections.Generic.HashSet<DBObject>();

                    foreach (DBDictionaryEntry dictEntry in groups)
                    {
                        cwConversor.Group cwGroup = new cwConversor.Group();
                        DBObject dbo = acTrans.GetObject(dictEntry.Value, OpenMode.ForRead);
                        Group group = dbo as Group;
                        ObjectId[] ids = group.GetAllEntityIds();
                        int l = ids.Length;
                        for (int i = 0; i < l; ++i )
                        {
                            ObjectId id2 = ids[i];
                            DBObject dboInG = acTrans.GetObject(id2, OpenMode.ForRead);
                            cwConversor.BaseObj obj = getCWObject(dboInG, id++);
                            usedObjects.Add(dboInG);
                            if(obj != null)
                                cwGroup.objects.Add( obj );
                        }
                        jsonDwg.groups.Add(cwGroup);
                    }

                                        // Step through each object in Model space and
                    // display the type of object found
                    foreach (ObjectId acObjId in acBlkTblRec)
                    {
                        DBObject dbo = acTrans.GetObject(acObjId, OpenMode.ForRead);

                        if (usedObjects.Contains(dbo))
                            continue;

                        cwConversor.BaseObj obj = getCWObject(dbo, id++);
                        if (obj != null)
                            jsonDwg.objects.Add( obj );
                    }

                    // Dispose of the transaction
                }

                string output = JsonConvert.SerializeObject(jsonDwg, Formatting.Indented);
                file.WriteLine(output);
            }
        }


        [CommandMethod("json2dwg")]
        public static void json2dwg()
        {
            Stream myStream = null;
            System.Windows.Forms.OpenFileDialog openFileDialog1 = new System.Windows.Forms.OpenFileDialog();

            openFileDialog1.InitialDirectory = "c:\\";
            openFileDialog1.Filter = "json files (*.json)|*.json|All files (*.*)|*.*";
            openFileDialog1.FilterIndex = 2;
            openFileDialog1.RestoreDirectory = true;

            string text = "";

            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                try
                {
                    if ((myStream = openFileDialog1.OpenFile()) != null)
                    {
                        StreamReader reader = new StreamReader(myStream);
                        text = reader.ReadToEnd();
                    }
                }
                catch (System.Exception ex)
                {
                    System.Windows.Forms.MessageBox.Show("Error: Could not read file from disk. Original error: " + ex.Message);
                }
            }

            JObject jsonDwg = JObject.Parse(text);
            json2dwgConverter.json2dwg(jsonDwg);
        }

    }

}
