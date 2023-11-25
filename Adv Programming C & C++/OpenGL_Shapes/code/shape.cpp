// $Id: shape.cpp,v 1.2 2019-02-28 15:24:20-08 - - $

#include <typeinfo>
#include <unordered_map>
using namespace std;

#include "shape.h"
#include "util.h"

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape() {
   DEBUGF ('c', this);
}

text::text (void* glut_bitmap_font_, const string& textdata_):
      glut_bitmap_font(glut_bitmap_font_), textdata(textdata_) {
   DEBUGF ('c', this);
}

ellipse::ellipse (GLfloat width, GLfloat height):
dimension ({width, height}) {
   DEBUGF ('c', this);
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}


polygon::polygon (const vertex_list& vertices_): vertices(vertices_) {
   DEBUGF ('c', this);
}

rectangle::rectangle (GLfloat width, GLfloat height):
      polygon({{width, height}, {width, 0}, {0,0}, {0, height}}) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

square::square (GLfloat width): rectangle (width, width) {
   DEBUGF ('c', this);
}

diamond::diamond (const GLfloat width, const GLfloat height): 
    polygon({{0,0},{width/2,height/2},{width,0},{width/2,0-height/2}}) {
    DEBUGF ('c', this);
}

triangle::triangle (const vertex_list& vertices_): polygon(vertices_) {
   DEBUGF ('c', this);
}

equilateral::equilateral(const GLfloat width): 
   triangle({{0,0}, {width / 2, width}, {width,0}}) {
   DEBUGF ('c', this);
}

void text::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glColor3ubv (color.ubvec);
   glRasterPos2f (center.xpos, center.ypos);
   glutBitmapString (glut_bitmap_font, 
               reinterpret_cast<const GLubyte*>(textdata.c_str()));
}

void text::draw_border (const vertex& center, const rgbcolor& color,
const GLfloat& width) const {
   size_t height = glutBitmapHeight (glut_bitmap_font);
   size_t size = glutBitmapLength (glut_bitmap_font, 
               reinterpret_cast<const GLubyte*> (textdata.c_str()));
   glColor3ubv (color.ubvec);
   glLineWidth(width);
   glBegin (GL_LINE_LOOP);
   glVertex2f (center.xpos+size+width,center.ypos+height+width);
   glVertex2f (center.xpos+size+width,center.ypos-width);
   glVertex2f (center.xpos-width,center.ypos-width);
   glVertex2f (center.xpos-width,center.ypos+height+width);
   glEnd();
   DEBUGF ('d', this << "(" << center << "," << color << ")");
}

void ellipse::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");

   glBegin(GL_POLYGON);
   glEnable(GL_LINE_SMOOTH);
   glColor3ubv(color.ubvec);
   const float point = 2 * M_PI / 32;
   for (float around = 0; around < 2 * M_PI; around += point) {
      glVertex2f (dimension.xpos * cos (around) + center.xpos,
                  dimension.ypos * sin (around) + center.ypos);
   }
   glEnd();
}


void ellipse::draw_border (const vertex& center, const rgbcolor& color,
const GLfloat& width) const{
   glLineWidth(width);
   glBegin(GL_LINE_LOOP);
   glEnable(GL_LINE_SMOOTH);
   glColor3ubv(color.ubvec);
   const float point = 2 * M_PI / 32;
   for (float around = 0; around < 2 * M_PI; around += point) {
      glVertex2f (dimension.xpos * cos (around) + center.xpos, 
                  dimension.ypos * sin (around) + center.ypos);
   }
   glEnd();
}

void polygon::draw (const vertex& center, const rgbcolor& color)const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   glBegin (GL_POLYGON);
   glColor3ubv (color.ubvec);
   for(auto vertex: vertices)
       glVertex2f (vertex.xpos+center.xpos,vertex.ypos+center.ypos);
   glEnd();
}

void polygon::draw_border (const vertex& center, const rgbcolor& color,
const GLfloat& width) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   if(vertices.empty()) return;
   glLineWidth(width);
   glBegin (GL_LINE_LOOP);
   glColor3ubv (color.ubvec);
   for(auto vertex: vertices) {
       glVertex2f (vertex.xpos+center.xpos,vertex.ypos+center.ypos);
   }
   
   glEnd();
}


void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}
