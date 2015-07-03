using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.Colors;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Geometry;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;

namespace AutoCAD_CSharp_plug_in1
{
    class json2dwgConverter
    {
        static AttachmentPoint str2AttachmentPoint(string attStr)
        {
            Dictionary<string, AttachmentPoint> attPoints = new Dictionary<string, AttachmentPoint>();
            attPoints["TopLeft"] = AttachmentPoint.TopLeft;
            attPoints["TopRight"] = AttachmentPoint.TopRight;
            attPoints["TopCenter"] = AttachmentPoint.TopCenter;
            attPoints["MiddleLeft"] = AttachmentPoint.MiddleLeft;
            attPoints["MiddleCenter"] = AttachmentPoint.MiddleCenter;
            attPoints["MiddleRight"] = AttachmentPoint.MiddleRight;
            attPoints["BaseLeft"] = AttachmentPoint.BaseLeft;
            attPoints["BaseCenter"] = AttachmentPoint.BaseCenter;
            attPoints["BaseRight"] = AttachmentPoint.BaseRight;

            if( attPoints.ContainsKey( attStr ) )
                return attPoints[attStr];
            else
                return AttachmentPoint.MiddleCenter;
        }

        static Entity readPolyline(JToken resultJtoken)
        {
            Polyline acPolyline = new Polyline();

            IList<JToken> points = resultJtoken["points"].Children().ToList();
            int counter = 0;
            foreach (JToken point in points)
            {
                double x = point["x"].Value<double>();
                double y = point["y"].Value<double>();
                Point2d pt = new Point2d(x, y);
                acPolyline.AddVertexAt( counter++, pt, 0, 0, 0 );
            }
            bool closed = resultJtoken["closed"].Value<bool>();
            acPolyline.Closed = closed;

            return acPolyline;
        }


        static Entity readLine(JToken resultJtoken)
        {
            double x1 = resultJtoken["start"]["x"].Value<double>();
            double y1 = resultJtoken["start"]["y"].Value<double>();

            double x2 = resultJtoken["end"]["x"].Value<double>();
            double y2 = resultJtoken["end"]["y"].Value<double>();


            Point3d ptStart = new Point3d(x1, y1, 0.0);
            Point3d ptEnd = new Point3d(x2, y2, 0.0);

            // Define the new line
            Line acLine = new Line(ptStart, ptEnd);

            return acLine;
        }


        static Entity readMText(JToken resultJtoken)
        {
            double x = resultJtoken["location"]["x"].Value<double>();
            double y = resultJtoken["location"]["y"].Value<double>();
            string text = resultJtoken["text"].Value<string>();
            double width = resultJtoken["width"].Value<double>();
            double height = resultJtoken["height"].Value<double>();
            double textHeight = resultJtoken["textHeight"].Value<double>();
            string attachment = resultJtoken["attachment"].Value<string>();
            
            Point3d location = new Point3d(x, y, 0.0);

            MText mText = new MText();
            mText.Location = location;
            mText.Height = height;
            mText.Width = width;
            mText.TextHeight = textHeight;
            mText.SetContentsRtf( text );
            mText.Attachment = str2AttachmentPoint(attachment);

            return mText;
        }


        static Entity readDBText(JToken resultJtoken)
        {
            double x = resultJtoken["location"]["x"].Value<double>();
            double y = resultJtoken["location"]["y"].Value<double>();
            string text = resultJtoken["text"].Value<string>();
            double widthFactor = resultJtoken["widthFactor"].Value<double>();
            double height = resultJtoken["height"].Value<double>();
            string justify = resultJtoken["justify"].Value<string>();

            Point3d location = new Point3d(x, y, 0.0);

            DBText dbText = new DBText();
            dbText.Height = height;
            dbText.WidthFactor = widthFactor;
            dbText.Position = location;
            dbText.TextString = text;
            dbText.Justify = str2AttachmentPoint(justify);

            return dbText;
        }


        static void addObj(BlockTableRecord acBlkTblRec, Transaction acTrans, JToken resultJtoken)
        {
            string type = "";
            try
            {
                type = resultJtoken.Value<string>("type") ?? "";
            }
            catch (System.Exception ex)
            {
                return;
            }

            Entity entity = null;
            if (type == "line")
                entity = readLine(resultJtoken);
            else if (type == "polyline")
                entity = readPolyline(resultJtoken);
            else if (type == "mtext")
                entity = readMText(resultJtoken);
            else if (type == "dbtext")
                entity = readDBText(resultJtoken);
            else
                return;

            //entity.Layer = resultJtoken["layer"].Value<string>();
            entity.SetDatabaseDefaults();

            // Add the line to the drawing
            acBlkTblRec.AppendEntity(entity);
            entity.Color = Color.FromRgb(0, 255, 0);
            acTrans.AddNewlyCreatedDBObject(entity, true);
        }


        public static void json2dwg(JObject jsonDwg)
        {
            // Get the current database and start the Transaction Manager
            Document acDoc = Application.DocumentManager.MdiActiveDocument;
            Database acCurDb = acDoc.Database;

            using (Transaction acTrans = acCurDb.TransactionManager.StartTransaction())
            {
                BlockTable acBlkTbl;
                BlockTableRecord acBlkTblRec;

                // Open Model space for write
                acBlkTbl = acTrans.GetObject(acCurDb.BlockTableId,
                                             OpenMode.ForRead) as BlockTable;

                acBlkTblRec = acTrans.GetObject(acBlkTbl[BlockTableRecord.ModelSpace], OpenMode.ForWrite) as BlockTableRecord;

                {
                    IList<JToken> objects = jsonDwg["objects"].Children().ToList();
                    foreach (JToken resultJtoken in objects)
                        addObj(acBlkTblRec, acTrans, resultJtoken);
                }

                //IList<JToken> groups = jsonDwg["groups"].Children().ToList();
                //foreach (JToken group in groups)
                //{
                //    IList<JToken> objects = group["groups"].Children().ToList();
                //    foreach (JToken resultJtoken in objects)
                //        addObj(acBlkTblRec, acTrans, resultJtoken);
                //}

                // Zoom to the extents or limits of the drawing
                acDoc.SendStringToExecute("._zoom _all ", true, false, false);

                // Commit the changes and dispose of the transaction
                acTrans.Commit();
            }
        }
    }
}

