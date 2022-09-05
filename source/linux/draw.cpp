#include "../../include/linux/rsgl.hpp" 
#include "deps/drawtext.hpp"
#include "deps/nanosvgrast.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.hpp"
#include <GL/gl.h>

// load font function
RSGL::Font::Font(std::string File, int Size){ 
    size=Size; 
    file=File; 
    if(!(font = dtx_open_font(file.c_str(), size))) 
        fprintf(stderr, "failed to open font\n"); 
}

// vector of cached loaded fonts
std::vector<RSGL::Font> fonts;

void RSGL::drawText(std::string text,RSGL::circle c,RSGL::Font font,RSGL::color col,drawArgs args/*={}*/){
    // draw scroll text
    if (args.scrollSpeed){
        static RSGL::circle sc = c; // static circle for moving when it scrolls
        int nl = 0; // count of new lines

        for (int i=0; i < text.size(); i++) 
            if (text.at(i) == '\n') nl++; //  count all the new lines 
        
        if (args.stopY == -1) 
            args.stopY = nl; // if stop = -1, stop whereever the text ends

        if ( abs(abs(sc.y-sc.y)/nl) <= args.stopY ) 
            sc.y--; // if the text can go down, move the text
        
        RSGL::drawText(text, {sc.x ,sc.y, sc.radius}, font, {255,0,0}); // draw
    }
    // draw typed-text
    else if (args.typeSpeed){
      static std::string outputText = ""; // static text to add to
      
      if (RSGL::timer(args.typeSpeed) && text.size() != outputText.size()) // wait for the tpe speed and check if the text is complete or not
          outputText += text.at(outputText.size()); // after waiting, add the new character
      
      RSGL::drawText(outputText, c, font, col);  // draw
    }
    // draw regular text
    else{
        RSGL::Font Font; // current font

        if (font.size != c.radius && font.file.size()){ // if the font is not loaded 
            bool loaded = false; 
            for (auto& fi : fonts){ 
                if (fi.file == font.file && fi.size == c.radius){ 
                    Font = fi;
                    loaded = true; 
                    
                    break; 
                }
            } // check if the font is in fonts

            if (!loaded){ // if it's not in fonts, load the font into fonts
                fonts.insert(fonts.end(), RSGL::Font(font.file, c.radius)); 
                Font = fonts.back(); 
            } 
        }

        dtx_use_font(Font.font, c.radius); // set the font

        glPushMatrix(); // text matrix
            // tooling 
            c.x -= 5; 
            c.y = args.win.r.h-(c.y+20);
            
            // change the x/y world cords to real cords
            glOrtho(0, args.win.r.w, 0, args.win.r.h, 0, 2);        
            glTranslatef(c.x, c.y , 0); // translate the cords
    
            glColor4f(col.r/255.0, col.g/255.0, col.b/255.0, col.a/255.0); // set the color to draw the text in
            dtx_string(text.c_str()); // draw the text
        glPopMatrix(); // text matrix end
    }
}

// draw a doted line function
void DrawDottedLine(RSGL::rect r,RSGL::color c,RSGL::window win,bool t = false) {
    if (t){
        DrawDottedLine({r.x, r.y, r.x+r.w, r.y}, c, win);
        DrawDottedLine({r.x, r.y, r.x, r.y+r.h}, c, win);
        DrawDottedLine({r.x+ r.w, r.y, r.x+r.w, r.y+r.h}, c, win);

        DrawDottedLine({r.x, r.y+r.h, r.x+r.w ,r.y+r.h}, c, win);
    }   
    else{
        int x0 = r.x, y0 = r.y, x1 = r.w, y1 = r.h;
        int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;
        int count = 0;

        glPushMatrix();
        if (r.rotationAngle){
            glRotatef(r.rotationAngle, 0, 0, 1);
            glTranslatef(dx, dy, 0);
        } glOrtho(0, win.r.w, 0, win.r.h, 0, 2);
        glBegin(GL_POINTS);
        while (1) {
            if (count < 10) {
                float x2 = x0;
                float y2 = y0;
                glColor4f(c.r/255.0, c.g/255.0, c.b/255.0, c.a/255.0);
                glVertex2f(x2, y2);
            }
            if (x0 == x1 && y0 == y1) break;

            e2 = 2*err;
            if (e2 > dy) { 
                err += dy; 
                x0 += sx; 
            }
            else if (e2 < dx) { 
                err += dx; 
                y0 += sy; 
            }
            
            count = (count + 1) % 20;
        }
        glEnd();
        glPopMatrix();
        glFlush();
    }
}

void RSGLdrawRoundRect(RSGL::rect r,RSGL::color c,bool fill, RSGL::window win){
    int radius = 2;
	
    if (fill) 
        RSGL::drawRect({r.x, r.y, r.w-1, r.h-1}, c, {.win=win});

    RSGL::drawLine({r.x + radius, r.y}, {r.x + r.w - (radius * 2), r.y}, c, win);
    RSGL::drawBoundCircle({r.x + r.w - (radius * 2)-2, r.y, radius+2 * 2}, c, {r.x + r.w - (radius * 2),-1,-1,r.y+3}, {-1,-1,-1,-1}, false ,win);
    RSGL::drawLine({r.x + r.w, r.y + radius}, {r.x + r.w, r.y + r.h - (radius * 2)},c,win);
    RSGL::drawBoundCircle({r.x + r.w - (radius * 2)-2 , r.y + r.h - (radius * 2)-2, radius+2 * 2},c,{r.x + r.w - (radius * 2) ,-1,r.y + r.h - (radius * 2),-1},{-1,-1,-1,-1},false,win);

	RSGL::drawLine({r.x + r.w - (radius * 2), r.y + r.h}, {r.x + radius, r.y + r.h},c,win);
    RSGL::drawBoundCircle({r.x, r.y + r.h - (radius * 2), radius * 2},c,{r.x,r.x+2, -1,-1},{-1,-1,-1,-1},false,win);

    RSGL::drawLine({r.x,r.y+r.h - (radius*2)}, {r.x,r.y+radius}, c,win);
    RSGL::drawBoundCircle({r.x-1,r.y,radius*2},c,{r.x,r.x+2,-1,r.y+2},{-1,-1,-1,-1},false,win);
}

void RSGL::drawRect(RSGL::rect r,color c, drawArgs args/*={}*/){
    // rounded rect
    if (args.rounded)
        RSGLdrawRoundRect(r, c, args.fill, args.win);
    
    // dotted rect
    else if (args.dotted)
        DrawDottedLine(r, c, args.win, true);

    // draw rect with opengl
    else if (args.win.GPU == 1){
        GLenum m = (args.fill) ? GL_POLYGON :  GL_LINE_STRIP; // switch the draw type if it's not filled
    
        glPushMatrix(); // start of opengl matrix
            
            if (r.rotationAngle){ // rotate if needed
                glRotatef(r.rotationAngle, 0, 0, 1);
                glTranslatef(r.x, r.y, 0);
            }  

            glOrtho(0, args.win.r.w, args.win.r.h, 0, 0, 2); // convert to world cords

            glBegin(m); // start drawing the rect
                glColor4f(c.r/255.0, c.g/255.0, c.b/255.0, c.a/255.0); // set the color of the rect
                
                glVertex2f(r.x,  r.y);
                glVertex2f(r.x+r.w, r.y);
                glVertex2f(r.x+r.w, r.y+r.h);
                glVertex2f(r.x, r.y+r.h); 
                if (!args.fill) glVertex2f(r.x,  r.y); // the final vertex point if it's line-basd
            glEnd(); // finish the rect

        glPopMatrix(); // end of matrix
        glFlush(); // flush the openGL buffer
    }
}

void RSGL::drawLine(RSGL::point p1, RSGL::point p2, RSGL::color c, RSGL::window win){
    if (win.GPU = 1){
		glPushMatrix(); // start of opengl matrix
			glOrtho(0, win.r.w, win.r.h, 0, 0, 2); // convert to world cords
			
			glBegin(GL_LINE_STRIP); // start drawing the line
				glColor4f(c.r/255.0, c.g/255.0, c.b/255.0, c.a/255.0); // set the color of the line

				glVertex2f(p1.x, p1.y); // starting point
				glVertex2f(p2.x, p2.y); // ending point
			glEnd(); // finish drawing
			glFlush(); // flush the openGL buffer
		glPopMatrix(); // start of opengl matrix
    }
}

void RSGL::drawTriangle(RSGL::triangle t, RSGL::color c, drawArgs args/*={}*/) {
    glPushMatrix(); // start of opengl matrix
        GLenum m = (args.fill) ? GL_TRIANGLES :  GL_LINE_LOOP; // switch the draw type if it's not filled
        glOrtho(0, args.win.r.w, args.win.r.h, 0, 0, 2); // convert cords to world cords

        glBegin(m); // start drawing
            glColor4f(c.r/255.0, c.g/255.0, c.b/255.0, c.a/255.0); // set color

            glVertex3f(t.points.at(2).x, t.points.at(2).y, 0);
            glVertex3f(t.points.at(0).x, t.points.at(0).y, 0);
            glVertex3f(t.points.at(1).x, t.points.at(1).y, 0);

        glEnd(); // end drawing
    
    glPopMatrix(); // end of opengl matrix
}

void RSGL::drawCircle(RSGL::circle c, color col, drawArgs args){
    if (args.win.GPU == 1){
        // uhm fix?
        c = {c.x + c.radius, c.y + c.radius, c.radius/2};

        // vars for calculations
        float theta = 3.1415926 * 2 / float(50);
        float tangetial_factor = tanf(theta);//calculate the tangential factor 

        float radial_factor = cosf(theta);//calculate the radial factor 

        glPushMatrix();
            glLineWidth(2); 

            glOrtho(0, args.win.r.w, args.win.r.h, 0, 0, 2); // convert to world cords

            glBegin(GL_LINE_LOOP); // start drawing
                glColor4f(col.r/255.0,col.g/255.0,col.b/255.0,col.a/255.0);
                for (int i=0; i < c.radius; i++){ // go through the layers
                    float x = c.radius-i; // circle draw x
                    float y = 0; // circle draw y

                    for (int ii = 0; ii < 50; ii++){
                        glVertex2f(x + c.x, y + c.y); //output vertex 
                        
                        // calculate x/y for the circle
                        float tx = -y; 
                        float ty = x;
                        
                        x += tx * tangetial_factor;
                        y += ty * tangetial_factor;
                        
                        x *= radial_factor;
                        y *= radial_factor;
                    } if (!args.fill) i=c.radius; // skip drawing the inner lays if it's not filled
                }
            glEnd(); // end drawing
        glPopMatrix();
    }
}

void RSGL::drawPoint(RSGL::point p, color c, RSGL::window win){ 
    RSGL::drawRect({p.x, p.y, 1, 1}, c, {.win=win}); // just draw a 1x1 rect  
}

std::vector<RSGL::image> tex; // cache for loading images

// load pre-loaded data into image cache
void RSGL::loadTex(std::vector<std::vector<unsigned char*>> texs){
	for (auto& res : texs){  // go through each vector
		// get the source w/h
        int w=500; 
        int h=500;

        // create an RSGL::image object, using res[0] as the file name
		RSGL::image img = { std::string((char*)res[0]) };

        // load res[1] into RSGL::image's openGL texture
        glGenTextures(1, &img.tex);
        glBindTexture(GL_TEXTURE_2D, img.tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h,0, GL_RGB, GL_UNSIGNED_BYTE, res[1]);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,0, GL_RGBA, GL_UNSIGNED_BYTE, res[1]);

		tex.insert(tex.end(), img); // send the final RSGL::image off to the texs cache
	}
}


RSGL::image RSGL::loadSVG(RSGL::image r){
    // init vars needed
    NSVGimage *image = NULL;
    NSVGrasterizer *rast = NULL;

    int w, h; // source image size
    
    image = nsvgParseFromFile(r.file.c_str(), "px", 96.0f); // load the file
    
    // grab width/height
    w = (int)image->width;
    h = (int)image->height;

    rast = nsvgCreateRasterizer(); // create rasterzing object for "flattening"

    unsigned char* img = (unsigned char*)malloc(w*h*4); // make data with room for the image
    
    nsvgRasterize(rast, image, 0,0,1, img, w, h, w*4); // load the flattened image into the data

    // load the data into a openGL texture
    glGenTextures(1, &r.tex);
    glBindTexture(GL_TEXTURE_2D, r.tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    
    // free any leftover data
    nsvgDeleteRasterizer(rast); 
    nsvgDelete(image);

    glBindTexture(GL_TEXTURE_2D, 0);
    return r;
} 

RSGL::image RSGL::loadImage(RSGL::image r){    
    int w,h,n; // source size / channels

    unsigned char *data = stbi_load(r.file.c_str(), &w, &h, &n, 0); // load the image with stbi
    
    glGenTextures(1, &r.tex);
    glBindTexture(GL_TEXTURE_2D, r.tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (!r.r.w && !r.r.h) 
        r.r = {r.r.x, r.r.y, w, h}; // default w/h if w/h are 0
        
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // load into a texture based on the ammount of channels
    if (n==3) 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
   
    else if (n==4)  
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data); // free the stbi data
    
    glBindTexture(GL_TEXTURE_2D, 0);

    return r;
}

void RSGL::drawImage(RSGL::image r, RSGL::window win){
    int xx = -1; // check indexs of texs this is

    // try to find if the image has been preloaded
    if (!tex.empty()) { 
        for (int i=0; i<tex.size(); i++) { 
            if (tex[i].file == r.file) {
                xx = i;
                break;
            } 
        }
    }

    if (xx==-1) { // load the image if it has not been preloaded

        // get the file extention
        std::string ext = r.file;
        ext.replace(ext.begin(), ext.begin() + ext.find_last_of("."), "");
        
        if (ext != ".svg") r = loadImage(r); // if it's not a .svg, load it with stbi
        else r = loadSVG(r); // else load it with nsvg

        // load the image into texs and set that as the current image
        tex.insert(tex.end(), r); 
        xx=tex.size()-1;
    }
    
    if (!r.r.w && !r.r.h) r.r = {r.r.x, r.r.y, tex[xx].r.w, tex[xx].r.h}; // use default sizes if w or h are 0

    glPushMatrix(); // start GL matrix

        if (r.r.rotationAngle){ // select the rotation
            glRotatef(r.r.rotationAngle, 0, 0, 1);
            glTranslatef(r.r.x, r.r.y, 0);
        }

        glOrtho(0, win.r.w, win.r.h, 0, 0, 2); // use world cords

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex[xx].tex);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        
        glBegin(GL_QUADS); // start drawing the image
            glTexCoord2i(0, 0); glVertex2f(r.r.x,  r.r.y);
            glTexCoord2i(1, 0); glVertex2f(r.r.x + r.r.w, r.r.y);
            glTexCoord2i(1, 1);  glVertex2f(r.r.x + r.r.w, r.r.y + r.r.h);
            glTexCoord2i(0, 1); glVertex2f(r.r.x, r.r.y + r.r.h);
        glEnd(); // end of drawing the image
    glPopMatrix(); // end gl matrix

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RSGL::drawImage(std::string fileName, RSGL::rect r, RSGL::window d){ 
    RSGL::drawImage({fileName, r}, d); // draw the data as a RSGL::image struct 
}

void RSGL::drawPoints(std::vector<RSGL::point> p, RSGL::color c, RSGL::window win){
    glBegin(GL_POINTS); // start drawing points
        glColor4f(c.r/255.0, c.g/255.0, c.b/255.0,c.a/255.0); // select color
        for (int i=0; i < p.size(); i++){ // go through the ooints
            glVertex2f(p.at(i).x, p.at(i).y); // draw point
        }
    glEnd(); // finish drawing
    glPopMatrix(); // end matrix
    
    glFlush(); // flush gl buffer
}

void RSGL::drawPoints(std::vector<RSGL::point> p,std::vector<RSGL::color> c, RSGL::window win){
    glPushMatrix(); // start matrix
        glOrtho(0, win.r.w, win.r.h, 0, 0, 2); // switch to world cords

        glBegin(GL_POINTS); // start drawing points
            for (int i=0; i < p.size(); i++){ // go through the ooints
                glColor4f(c.at(i).r/255.0, c.at(i).g/255.0, c.at(i).b/255.0, c.at(i).a/255.0); // select color
                glVertex2f(p.at(i).x, p.at(i).y); // draw point
            }
        glEnd(); // finish drawing
    glPopMatrix(); // end matrix
    
    glFlush(); // flush gl buffer
}

// who cares, this really should only be used in RSGL sources only
void RSGL::drawBoundCircle(RSGL::circle c, RSGL::color col, RSGL::rect boundary,RSGL::rect noHIT,bool fill,RSGL::window win){
    bool noHit= noHIT.x+noHIT.y+noHIT.w+noHIT.h != -1;

    c.radius /= 2; c.x+=c.radius; c.y+=c.radius;

    float theta = 3.1415926 * 2 / float(50);
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    glPushMatrix();
    glLineWidth(2);
    glOrtho(0, win.r.w, win.r.h, 0, 0, 2);

    glBegin(GL_LINE_LOOP);
    glColor4f(col.r/255.0,col.g/255.0,col.b/255.0,col.a/255.0);
    for (int i=0; i < c.radius; i++){
        float x = c.radius-i;
        float y = 0;

        for (int ii = 0; ii < 50; ii++)
        {
            int newX= x + c.x, newY=y + c.y;
            if ( ( boundary.x == -1 ||  newX >= boundary.x) &&  
            (boundary.y == -1 || newX <= boundary.y ) &&
            ( boundary.w == -1 || ( newY >= boundary.w)) && 
            (boundary.h == -1 || newY <= boundary.h ) &&
            (!noHit || noHit && !RSGL::RectCollidePoint(noHIT,{newX,newY}))) glVertex2f(newX, newY);//output vertex 
            
            float tx = -y;
            float ty = x;
            
            x += tx * tangetial_factor;
            y += ty * tangetial_factor;
            
            x *= radial_factor;
            y *= radial_factor;
        } if (!fill) i=c.radius;
    }
    glEnd();
    glLineWidth(1);
    glPopMatrix();
    glFlush(); 
}
