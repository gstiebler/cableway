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
        // The CommandMethod attribute can be applied to any public  member 
        // function of any public class.
        // The function should take no arguments and return nothing.
        // If the method is an intance member then the enclosing class is 
        // intantiated for each document. If the member is a static member then
        // the enclosing class is NOT intantiated.
        //
        // NOTE: CommandMethod has overloads where you can provide helpid and
        // context menu.

        // Modal Command with localized name
        [CommandMethod("MyGroup", "MyCommand", "MyCommandLocal", CommandFlags.Modal)]
        public void MyCommand() // This method can have any name
        {
            // Get the current database and start the Transaction Manager
            Document acDoc = Application.DocumentManager.MdiActiveDocument;
            Database acCurDb = acDoc.Database;

            PromptPointResult pPtRes;
            PromptPointOptions pPtOpts = new PromptPointOptions("");

            // Prompt for the start point
            pPtOpts.Message = "\nEnter the start point of the line: ";
            pPtRes = acDoc.Editor.GetPoint(pPtOpts);
            Point3d ptStart = pPtRes.Value;

            // Exit if the user presses ESC or cancels the command
            if (pPtRes.Status == PromptStatus.Cancel) return;

            // Prompt for the end point
            pPtOpts.Message = "\nEnter the end point of the line: ";
            pPtOpts.UseBasePoint = true;
            pPtOpts.BasePoint = ptStart;
            pPtRes = acDoc.Editor.GetPoint(pPtOpts);
            Point3d ptEnd = pPtRes.Value;

            if (pPtRes.Status == PromptStatus.Cancel) return;

            // Start a transaction
            using (Transaction acTrans = acCurDb.TransactionManager.StartTransaction())
            {
                BlockTable acBlkTbl;
                BlockTableRecord acBlkTblRec;

                // Open Model space for write
                acBlkTbl = acTrans.GetObject(acCurDb.BlockTableId,
                                             OpenMode.ForRead) as BlockTable;

                acBlkTblRec = acTrans.GetObject(acBlkTbl[BlockTableRecord.ModelSpace],
                                                OpenMode.ForWrite) as BlockTableRecord;

                // Define the new line
                Line acLine = new Line(ptStart, ptEnd);
                acLine.SetDatabaseDefaults();

                // Add the line to the drawing
                acBlkTblRec.AppendEntity(acLine);
                acTrans.AddNewlyCreatedDBObject(acLine, true);

                // Zoom to the extents or limits of the drawing
                acDoc.SendStringToExecute("._zoom _all ", true, false, false);

                // Commit the changes and dispose of the transaction
                acTrans.Commit();
            }
        }

        // Modal Command with pickfirst selection
        [CommandMethod("MyGroup", "MyPickFirst", "MyPickFirstLocal", CommandFlags.Modal | CommandFlags.UsePickSet)]
        public void MyPickFirst() // This method can have any name
        {
            PromptSelectionResult result = Application.DocumentManager.MdiActiveDocument.Editor.GetSelection();
            if (result.Status == PromptStatus.OK)
            {
                // There are selected entities
                // Put your command using pickfirst set code here
            }
            else
            {
                // There are no selected entities
                // Put your command code here
            }
        }

        // Application Session Command with localized name
        [CommandMethod("MyGroup", "MySessionCmd", "MySessionCmdLocal", CommandFlags.Modal | CommandFlags.Session)]
        public void MySessionCmd() // This method can have any name
        {
            // Put your command code here
        }

        // LispFunction is similar to CommandMethod but it creates a lisp 
        // callable function. Many return types are supported not just string
        // or integer.
        [LispFunction("MyLispFunction", "MyLispFunctionLocal")]
        public int MyLispFunction(ResultBuffer args) // This method can have any name
        {
            // Put your command code here

            // Return a value to the AutoCAD Lisp Interpreter
            return 1;
        }


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


        public static void writeJsonInteger(System.IO.StreamWriter file, string key, int number)
        {
            file.WriteLine("\"" + key + "\": " + number + ",");
        }


        public static void writeCircle(System.IO.StreamWriter file, Circle circle)
        {
            writeJsonString(file, "OBJ", "CIRCLE");
            writeJsonString(file, "LAYER", circle.Layer);
            writeJsonNumber(file, "DIAMETER", circle.Diameter, true);
            writeJsonNumber(file, "CENTER_X", circle.Center.X, true);
            writeJsonNumber(file, "CENTER_Y", circle.Center.Y, false);
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
            writeJsonNumber(file, "END_ANGLE", arc.EndAngle, false);
        }


        
        public static void writeLine(System.IO.StreamWriter file, Line line)
        {
            writeJsonString(file, "OBJ", "LINE");
            writeJsonString(file, "LAYER", line.Layer);
            writeJsonNumber(file, "START_POINT_X", line.StartPoint.X, true);
            writeJsonNumber(file, "START_POINT_Y", line.StartPoint.Y, true);
            writeJsonNumber(file, "END_POINT_X", line.EndPoint.X, true);
            writeJsonNumber(file, "END_POINT_Y", line.EndPoint.Y, false);
        }



        public static void writePolyline(System.IO.StreamWriter file, Polyline pline)
        {
            writeJsonString(file, "OBJ", "PLINE");
            writeJsonString(file, "LAYER", pline.Layer);
            writeJsonInteger(file, "NUM_VERTEX", pline.NumberOfVertices);
            file.WriteLine("[");
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
            file.WriteLine("]");
        }



        public static string replaceBreakString( string input )
        {
            return input.Replace( "\r\n", " " );
        }



        public static void writeText(System.IO.StreamWriter file, MText text)
        {
            writeJsonString(file, "OBJ", "TEXT");
            writeJsonString(file, "LAYER", text.Layer);
            writeJsonString(file, "TEXT", replaceBreakString(text.Text));
            writeJsonNumber(file, "X", text.Location.X, true);
            writeJsonNumber(file, "Y", text.Location.Y, true);
            writeJsonNumber(file, "WIDTH_FACTOR", text.Width, false);
        }



        public static void writeDBText(System.IO.StreamWriter file, DBText text)
        {
            writeJsonString(file, "OBJ", "DBTEXT");
            writeJsonString(file, "LAYER", text.Layer);
            writeJsonString(file, "TEXT", replaceBreakString(text.TextString));
            writeJsonNumber(file, "X", text.Position.X, true);
            writeJsonNumber(file, "Y", text.Position.Y, true);
            writeJsonNumber(file, "WIDTH_FACTOR", text.WidthFactor, false);
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
                file.WriteLine("\"__type not processed\": \"" + dbo.GetType() + "\"" );
            writeJsonInteger(file, "ID", id);
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

        
        [CommandMethod("ExportCW")]
        public static void ExportCW()
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
                            writeObject(file, dboInG, id++);
                            usedObjects.Add(dboInG);
                        }
                        file.WriteLine("],");
                    }
                    file.WriteLine("{}],");

                    int nCnt = 0;

                    file.WriteLine("\"objects\": [");
                    // Step through each object in Model space and
                    // display the type of object found
                    foreach (ObjectId acObjId in acBlkTblRec)
                    {
                        DBObject dbo = acTrans.GetObject(acObjId, OpenMode.ForRead);

                        if (usedObjects.Contains(dbo))
                            continue;

                        writeObject(file, dbo, id++);

                        nCnt = nCnt + 1;
                    }
                    file.WriteLine("{}]");
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
