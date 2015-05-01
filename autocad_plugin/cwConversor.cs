using Autodesk.AutoCAD.Geometry;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cwConversor
{
    public class Group
    {
        public ArrayList objects;

        public Group()
        {
            objects = new ArrayList();
        }
    }

    public class BaseObj
    {
        public string layer;
        public int id;

        public BaseObj(Autodesk.AutoCAD.DatabaseServices.Entity entity, int id_)
        {
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
            : base(line, id)
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
            : base(circle, id)
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
            : base(arc, id)
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

        public Polyline(Autodesk.AutoCAD.DatabaseServices.Polyline polyline, int id)
            : base(polyline, id)
        {
            points = new Point[polyline.NumberOfVertices];
            for (int i = 0; i < polyline.NumberOfVertices; i++)
                points[i] = new Point(polyline.GetPoint2dAt(i));
        }
    }

    public class Text : BaseObj
    {
        public Point location;
        public string text;
        public double factor;

        public Text(Autodesk.AutoCAD.DatabaseServices.MText textObj, int id)
            : base(textObj, id)
        {
            location = new Point(textObj.Location);
            text = textObj.Text;
            factor = textObj.Width;
        }

        public Text(Autodesk.AutoCAD.DatabaseServices.DBText textObj, int id)
            : base(textObj, id)
        {
            location = new Point(textObj.Position);
            text = textObj.TextString;
            factor = textObj.WidthFactor;
        }
    }

    public class JsonDwg : Group
    {
        public ArrayList groups;

        public JsonDwg()
        {
            groups = new ArrayList();
        }
    }
}
