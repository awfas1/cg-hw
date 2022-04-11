//
// copyright 2018 Palestine Polytechnic Univeristy
//
// This software can be used and/or modified for academich use as long as 
// this commented part is listed
//
// Last modified by: Zein Salah, on 26.02.2019
//


#include "RenderWidget.h"
#include <glut.h>
#include <QPainter>




RenderWidget::RenderWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}


RenderWidget::~RenderWidget()
{

}


QSize RenderWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}


QSize RenderWidget::sizeHint() const
{
    return QSize(600, 600);
}


void RenderWidget::initializeGL()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 600.0, 0.0, 600.0);

    //glOrtho(-2.0, 2.0, -2.0, 2.0, -100, 100);
    //gluPerspective(25.0, 1.0, 1.0, 100.0);

}



double xmin = 100, ymin = 200, xmax = 280, ymax = 380;
const int LEFT = 1; 
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;





int ComputeOutCode(double x, double y)
{



    int code = 0;
    if (y > ymax) //above the clip window
        code |= TOP;
    else if (y < ymin) //below the clip window
        code |= BOTTOM;
    if (x > xmax) //to the right of clip window
        code |= RIGHT;
    else if (x < xmin) //to the left of clip window
        code |= LEFT;
    return code; //return the calculated code
}

void CohenSutherland(double x0, double y0, double x1, double y1)
{


    int outcode0, outcode1, outcodeOut;
    bool accept = false, done = false;
    outcode0 = ComputeOutCode(x0, y0); //calculate the region of 1st point
    outcode1 = ComputeOutCode(x1, y1); //calculate the region of 2nd point
    do
    {
        if (!(outcode0 | outcode1))
        {
            accept = true; //both the points
            done = true; 
        }
        else if (outcode0 & outcode1)
            done = true; //both are outside
        else
        {
            double x, y;
            double m = (y1 - y0) / (x1 - x0);
            outcodeOut = outcode0 ? outcode0 : outcode1;

                if (outcodeOut & TOP)
                {
                    x = x0 + (1 / m) * (ymax - y0);
                    y = ymax;
                }
                else if (outcodeOut & BOTTOM)
                {
                    x = x0 + (1 / m) * (ymin - y0);
                    y = ymin;
                }
                else if (outcodeOut & RIGHT)
                {
                    y = y0 + m * (xmax - x0);
                    x = xmax;
                }
                else
                {
                    y = y0 + m * (xmin - x0);
                    x = xmin;
                }
            /* Intersection calculations are done,
           go ahead and mark the clipped line */
            if (outcodeOut == outcode0)
            {
                x0 = x;
                y0 = y;
                outcode0 = ComputeOutCode(x0, y0);
            }
            else
            {
                x1 = x;
                y1 = y;
                outcode1 = ComputeOutCode(x1, y1);
            }
        }
    } while (!done);

   
    glColor3f(1, 0, 0); 
    glBegin(GL_LINES);
    glVertex2d(x0, y0);
    glVertex2d(x1, y1);
    glEnd();
}

void Clip_Display()
{
    

    double x0 = 100, y0 = 150, x1 = 400, y1 = 450;    //glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2d(x0, y0);
    glVertex2d(x1, y1);

    glEnd();
    glBegin(GL_LINE_LOOP); // draw the clipping / viewing rectangle window
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();
    CohenSutherland(x0, y0, x1, y1); // call the algorithm
    glFlush(); // show the output

}

int LE[600], RE[600];

void Intersection(float x1, float y1, float x2, float y2)
{

    float x, M;
    int t, y;
    if (y1 > y2)
    {
        t = x1;
        x1 = x2;
        x2 = t;

        t = y1;
        y1 = y2;
        y2 = t;
    }

    if ((y2 - y1) == 0)
        M = (x2 - x1);
    else
        M = (x2 - x1) / (y2 - y1);

    x = x1;
    for (y = y1; y <= y2; y++)
    {
        if (x < LE[y])
            LE[y] = x;
        if (x > RE[y])
            RE[y] = x;

        x = x + M;
    }
}

void Poly_filling_ScanLine() {
    int x, y, i;
    int vx[6] = { 290,430,560,485,290,430 };
    int vy[6] = { 220,180,290,510,390,365 };


    for (i = 0; i < 600; i++)
    {
        LE[i] = 600;
        RE[i] = 0;
    }

    glColor3f(0, 0, 0);

    glBegin(GL_LINE_LOOP);
    for (int k = 0; k <= 5; k++)
        glVertex2i(vx[k], vy[k]);
    glEnd();

    for (int j = 0; j < 5; j++) {
        Intersection(vx[j], vy[j], vx[j + 1], vy[j + 1]);
    }
    Intersection(vx[5], vy[5], vx[0], vy[0]);





    for (y = 0; y < 600; y++)
    {
        if (LE[y] < RE[y]) {

            for (x = LE[y]; x < RE[y]; x++)
            {
                glBegin(GL_POINTS);
                glColor3f(0, 0, 1);
                glVertex2i(x, y);
                glEnd();
                glFlush();
            }
        }
    }
}





void RenderWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT);

    Poly_filling_ScanLine();

    Clip_Display();



   
}




void RenderWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

//    glViewport(0, 0, width, height);
}


