using Autodesk.AutoCAD.Geometry;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cwConversor
{
    public class Group
    {
        public List<BaseObj> objects;

        public Group()
        {
            objects = new List<BaseObj>();
        }
    }

    public class BaseObj
    {
        public string layer;
        public int id;
        public string type;

        public BaseObj(string type_, Autodesk.AutoCAD.DatabaseServices.Entity entity, int id_)
        {
            type = type_;
            layer = entity.Layer;
            id = id_;
        }
    }

    public class Point
    {
        public double x;
        public double y;

        public Point(Point3d p)
        {
            x = p.X;
            y = p.Y;
        }

        public Point(Point2d p)
        {
            x = p.X;
            y = p.Y;
        }
    }

    public class Line : BaseObj
    {
        public Point start;
        public Point end;

        public Line(Autodesk.AutoCAD.DatabaseServices.Line line, int id)
            : base("line", line, id)
        {
            start = new Point(line.StartPoint);
            end = new Point(line.EndPoint);
        }
    }

    public class Circle : BaseObj
    {
        public double diameter;
        public Point center;

        public Circle(Autodesk.AutoCAD.DatabaseServices.Circle circle, int id)
            : base("circle", circle, id)
        {
            diameter = circle.Diameter;
            center = new Point(circle.Center);
        }
    }

    public class Arc : BaseObj
    {
        public Point center;
        public Point endPoint;
        public double startAngle;
        public double endAngle;

        public Arc(Autodesk.AutoCAD.DatabaseServices.Arc arc, int id)
            : base("arc", arc, id)
        {
            center = new Point(arc.Center);
            endPoint = new Point(arc.EndPoint);
            startAngle = arc.StartAngle;
            endAngle = arc.EndAngle;
        }
    }

    public class Polyline : BaseObj
    {
        public Point[] points;
        public bool closed;

        public Polyline(Autodesk.AutoCAD.DatabaseServices.Polyline polyline, int id)
            : base("polyline", polyline, id)
        {
            points = new Point[polyline.NumberOfVertices];
            for (int i = 0; i < polyline.NumberOfVertices; i++)
                points[i] = new Point(polyline.GetPoint2dAt(i));
            closed = polyline.Closed;
        }
    }

    public class Text : BaseObj
    {
        public Point location;
        public string text;

        public Text(string type, Autodesk.AutoCAD.DatabaseServices.Entity textObj, int id, string text_, Point location_) 
            : base( type, textObj, id )
        {
            text = text_;
            location = location_;
        }
    }

    public class MText : Text
    {
        public double width;
        public double height;
        public double textHeight;
        public string attachment;

        public MText(Autodesk.AutoCAD.DatabaseServices.MText textObj, int id)
            : base("mtext", textObj, id, textObj.Text, new Point( textObj.Location ) )
        {
            width = textObj.Width;
            height = textObj.Height;
            textHeight = textObj.TextHeight;
            attachment = textObj.Attachment.ToString();
        }

    }

    public class DBText : Text
    {
        public double widthFactor;
        public double height;
        public string justify;

        public DBText(Autodesk.AutoCAD.DatabaseServices.DBText textObj, int id)
            : base("dbtext", textObj, id, textObj.TextString, new Point( textObj.Position ) )
        {
            widthFactor = textObj.WidthFactor;
            height = textObj.Height;
            justify = textObj.Justify.ToString();
        }
    }

    public class JsonDwg : Group
    {
        public List<Group> groups;

        public JsonDwg()
        {
            groups = new List<Group>();
        }
    }
}
