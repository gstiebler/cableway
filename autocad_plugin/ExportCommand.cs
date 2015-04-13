// (C) Copyright 2014 by  
//
using System;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.EditorInput;

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

        public static void writeJsonString( System.IO.StreamWriter file, string key, string value )
        {
            file.WriteLine("\"" + key + "\": " + "\"" + value + "\",");
        }


        public static void writeJsonNumber(System.IO.StreamWriter file, string key, double number, bool hasFinalComma)
        {
            string text = "\"" + key + "\": " + number;
            if(hasFinalComma)
                text += ",";
            file.WriteLine(text);
        }


        public static void writeJsonInteger(System.IO.StreamWriter file, string key, int number, bool hasFinalComma)
        {
            string text = "\"" + key + "\": " + number;
            if (hasFinalComma)
                text += ",";
            file.WriteLine(text);
        }


        public static void writeCircle(System.IO.StreamWriter file, Circle circle)
        {
            writeJsonString(file, "OBJ", "CIRCLE");
            writeJsonString(file, "LAYER", circle.Layer);
            writeJsonNumber(file, "DIAMETER", circle.Diameter, true);
            writeJsonNumber(file, "CENTER_X", circle.Center.X, true);
            writeJsonNumber(file, "CENTER_Y", circle.Center.Y, true);
        }



        public static void writeArc(System.IO.StreamWriter file, Arc arc)
        {
            writeJsonString(file, "OBJ", "ARC");
            writeJsonString(file, "LAYER", arc.Layer);
            writeJsonNumber(file, "CENTER_X", arc.Center.X, true);
            writeJsonNumber(file, "CENTER_Y", arc.Center.Y, true);
            writeJsonNumber(file, "END_POINT_X", arc.EndPoint.X, true);
            writeJsonNumber(file, "END_POINT_Y", arc.EndPoint.Y, true);
            writeJsonNumber(file, "START_ANGLE", arc.StartAngle, true);
            writeJsonNumber(file, "END_ANGLE", arc.EndAngle, true);
        }


        
        public static void writeLine(System.IO.StreamWriter file, Line line)
        {
            writeJsonString(file, "OBJ", "LINE");
            writeJsonString(file, "LAYER", line.Layer);
            writeJsonNumber(file, "START_POINT_X", line.StartPoint.X, true);
            writeJsonNumber(file, "START_POINT_Y", line.StartPoint.Y, true);
            writeJsonNumber(file, "END_POINT_X", line.EndPoint.X, true);
            writeJsonNumber(file, "END_POINT_Y", line.EndPoint.Y, true);
        }



        public static void writePolyline(System.IO.StreamWriter file, Polyline pline)
        {
            writeJsonString(file, "OBJ", "PLINE");
            writeJsonString(file, "LAYER", pline.Layer);
            writeJsonInteger(file, "NUM_VERTEX", pline.NumberOfVertices, true);
            file.WriteLine("\"vertices\": [");
            for (int i = 0; i < pline.NumberOfVertices; i++)
            {
                double x = pline.GetPoint2dAt(i).X;
                double y = pline.GetPoint2dAt(i).Y;
                file.WriteLine("{");
                writeJsonNumber(file, "X", x, true);
                writeJsonNumber(file, "Y", y, false);
                if (i == pline.NumberOfVertices - 1)
                    file.WriteLine("}");
                else
                    file.WriteLine("},");
            }
            file.WriteLine("],");
        }



        public static string replaceBreakString( string input )
        {
            return input.Replace( "\r\n", " " );
        }


        public static string replaceAspasString(string input)
        {
            return input.Replace("\"", "<*aspas*>");
        }


        public static string normalizeOutputText(string input)
        {
            return replaceAspasString( replaceBreakString(input) );
        }


        public static void writeText(System.IO.StreamWriter file, MText text)
        {
            writeJsonString(file, "OBJ", "TEXT");
            writeJsonString(file, "LAYER", text.Layer);
            writeJsonString(file, "TEXT", normalizeOutputText(text.Text));
            writeJsonNumber(file, "X", text.Location.X, true);
            writeJsonNumber(file, "Y", text.Location.Y, true);
            writeJsonNumber(file, "WIDTH_FACTOR", text.Width, true);
        }



        public static void writeDBText(System.IO.StreamWriter file, DBText text)
        {
            writeJsonString(file, "OBJ", "DBTEXT");
            writeJsonString(file, "LAYER", text.Layer);
            writeJsonString(file, "TEXT", normalizeOutputText(text.TextString));
            writeJsonNumber(file, "X", text.Position.X, true);
            writeJsonNumber(file, "Y", text.Position.Y, true);
            writeJsonNumber(file, "WIDTH_FACTOR", text.WidthFactor, true);
        }


        public static void writeObject(System.IO.StreamWriter file, DBObject dbo, int id, bool hasFinalComma)
        {
            file.WriteLine("{");
            if (dbo is Circle)
            {
                Circle circle = dbo as Circle;
                writeCircle(file, circle);
            }
            else if (dbo is Arc)
            {
                Arc arc = dbo as Arc;
                writeArc(file, arc);
            }
            else if (dbo is Line)
            {
                Line line = dbo as Line;
                writeLine(file, line);
            }
            else if (dbo is MText)
            {
                MText text = dbo as MText;
                writeText( file, text );
            }
            else if (dbo is DBText)
            {
                DBText text = dbo as DBText;
                writeDBText(file, text);
            }
            else if (dbo is Polyline)
            {
                Polyline pline = dbo as Polyline;
                writePolyline( file, pline );
            }
            else
                file.WriteLine("\"__type not processed\": \"" + dbo.GetType() + "\"," );
            writeJsonInteger(file, "ID", id, false);
            if (hasFinalComma)
                file.WriteLine("},");
            else
                file.WriteLine("}");
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

                    file.WriteLine("{ \"groups\": [");
                    int counter1 = 0;
                    foreach (DBDictionaryEntry dictEntry in groups)
                    {
                        DBObject dbo = acTrans.GetObject(dictEntry.Value, OpenMode.ForRead);
                        Group group = dbo as Group;
                        ObjectId[] ids = group.GetAllEntityIds();
                        int l = ids.Length;
                        file.WriteLine("[");
                        for (int i = 0; i < l; ++i )
                        {
                            ObjectId id2 = ids[i];
                            DBObject dboInG = acTrans.GetObject(id2, OpenMode.ForRead);
                            writeObject(file, dboInG, id++, i < (l - 1));
                            usedObjects.Add(dboInG);
                        }
                        counter1++;
                        if (counter1 == groups.Count)
                            file.WriteLine("]");
                        else
                            file.WriteLine("],");
                    }
                    file.WriteLine("],");

                    int nCnt = 0;

                    file.WriteLine("\"objects\": [");
                    // Step through each object in Model space and
                    // display the type of object found
                    foreach (ObjectId acObjId in acBlkTblRec)
                    {
                        DBObject dbo = acTrans.GetObject(acObjId, OpenMode.ForRead);

                        if (usedObjects.Contains(dbo))
                            continue;
                        
                        nCnt = nCnt + 1;
                        writeObject(file, dbo, id++, true);
                    }
                    file.WriteLine("{\"dummy\": \"dummy\"}]");
                    file.WriteLine("}");

                    // If no objects are found then display a message
                    if (nCnt == 0)
                    {
                        file.WriteLine("\n No objects found");
                    }

                    // Dispose of the transaction
                }
            }


           
        }

    }

}
