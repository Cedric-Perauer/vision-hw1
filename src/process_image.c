#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"


float get_pixel(image im, int x, int y, int c)
{
    if( x < 0){
        x = 0;
    }
    if( y < 0){
        y = 0;
    }
    if(x >= im.w){
        x= im.w-1;
    }
      if(y >= im.h){
        y= im.h-1;
    }
    if(c < 0){
        c = 0;

    }
    if(c > 2){
        c = 2;
    }
    return im.data[(c*im.w*im.h)+(y*im.w)+(x)];
}

 
void set_pixel(image im, int x, int y, int c, float v)
{
    

     if ( x < 0) {
        x = 0;
    } else if (x >= im.w)  {
        x = im.w - 1; 
    }

    if(y < 0) {
        y = 0;
    } else if (y >= im.h) {
        y = im.h - 1;
    }

    if (c < 0) {
        c = 0;
    } else if (c >= im.c) {
        c = im.c - 1;
    }

    int i = x + y * im.w + c * im.w * im.h;
    im.data[i] = v;
}


image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    for(int i = 0; i < im.h*im.w*im.c; i++){

        copy.data[i] = im.data[i];
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for(int i = 0; i < im.h*im.w; i++){
        gray.data[i] = im.data[i] * 0.299 + im.data[i+(im.h*im.w)] * 0.587  + 0.114 * im.data[i+(2*im.h*im.w)];
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for(int i = 0; i < im.h*im.w; i++){
        im.data[(c*im.h*im.w)+i] += v;
    }
}

void clamp_image(image im)
{ 
    for(int i = 0; i < im.c*im.h*im.w; i++){
        if(im.data[i] < 0){
            im.data[i] = 0;
        } 
        if(im.data[i] > 1){
            im.data[i] = 1;
        } 
    }
       
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

/* 
void rgb_to_hsv(image im)
{    
    float sat;
    float value;
    float min;
    float H_d = 0.0;
    float H;
    float diff;
    
    for(int i = 0; i < im.h; i++){

       value = three_way_max(im.data[i], im.data[i+im.h*im.w], im.data[i+2*im.h*im.w]); //V
       min = three_way_min(im.data[i], im.data[i+im.h*im.w], im.data[i+2*im.h*im.w]); 
       diff = value-min; //C 
      
       
      if(value==0.0){
            sat = 0.0;
       }
       else {
           sat = diff / value; //S
       }
     
       
       //Circle 
       
      
       if(diff==0){
          H_d = 0;
       }
       
       else if(value==im.data[i]){
       H_d = (im.data[i+im.h*im.w]-im.data[i+2*im.h*im.w])/diff; //Case V = R
       }
       else if(value==im.data[i+im.h*im.w]){
           H_d = ((im.data[i+2*im.h*im.w]-im.data[i])/diff) +2; //Case V = G
       }
       else if(value==im.data[i+2*im.h*im.w]){
           H_d = ((im.data[i]-im.data[i+im.h*im.w])/diff) + 4; //Case V = B
       }
       if(H_d < 0){
           H = H_d/6.0 + 1;
       }
       else {
            H = H_d/6.0;
       }
       im.data[i] = H;
       im.data[i+im.h*im.w] = sat;
       im.data[i+2*im.h*im.w] = value;
    }
}
*/

void rgb_to_hsv(image im)
{    
    float sat;
    float value;
    float min;
    float H_d = 0.0;
    float H;
    float diff;
    float r,g,b;
    int i,j;
    // i = i, j = jumn 
    for(i = 0; i < im.h; i++){
    for(j= 0; j < im.w; j++){
        r = get_pixel(im,j,i,0);
        g = get_pixel(im,j,i,1);
        b = get_pixel(im,j,i,2);
       value = three_way_max(r, g, b); //V
       min = three_way_min(r, g, b); 
       diff = value-min; //C 
      
       
      if(value==0.0){
            sat = 0.0;
       }
       else {
           sat = diff / value; //S
       }
     
       
       //Circle 
       
      
       if(diff==0){
          H_d = 0;
       }
       
       else if(value==r){
       H_d = (g-b)/diff; //Case V = R
       }
       else if(value==g){
           H_d = ((b-r)/diff) +2; //Case V = G
       }
       else if(value==b){
           H_d = ((r-g)/diff) + 4; //Case V = B
       }
       if(H_d < 0){
           H = H_d/6.0 + 1;
       }
       else {
            H = H_d/6.0;
       }
       set_pixel(im,j,i,0,H);
       set_pixel(im,j,i,1,sat);
       set_pixel(im,j,i,2,value);
    }
   
    }
}

void hsv_to_rgb(image im)
{ //channels : 1. H, 2. S, 3. V
  float H,S,V,h;
    float r,g,b;
    float diff;
    int i,j;
    float rgb[3],C,X;
    
    for(i = 0; i < im.h; i++){
    for(j= 0; j < im.w; j++){
        h = get_pixel(im,j,i,0);
        S = get_pixel(im,j,i,1);
        V = get_pixel(im,j,i,2);
     
      
       
            H = h * 6.0;
            C = V * S;
            X = C * (1 - fabs(fmod(H, 2.0) - 1));

             

             if(0 <= H && H <= 1.0) {
                rgb[0] = C;
                rgb[1] = X;
                rgb[2] = 0.0;
            } else if (1.0 <= H && H <= 2.0){
                rgb[0] = X;
                rgb[1] = C;
                rgb[2] = 0.0;
            } else if (2.0 <= H && H <= 3.0) {
                rgb[0] = 0.0;
                rgb[1] = C;
                rgb[2] = X;
            } else if (3.0 <= H && H <= 4.0) {
                rgb[0] = 0.0;
                rgb[1] = X;
                rgb[2] = C;
            } else if (4.0 <= H && H <= 5.0) {
                rgb[0] = X;
                rgb[1] = 0.0;
                rgb[2] = C;
            } else if (5.0 <= H && H <= 6.0) {
                rgb[0] = C;
                rgb[1] = 0.0;
                rgb[2] = X;
            } else {
                rgb[0] = 0.0;
                rgb[1] = 0.0;
                rgb[2] = 0.0;
            }

            diff = V - C;

            r = rgb[0] + diff;
            set_pixel(im, j, i, 0, r);

            g = rgb[1] + diff;
            set_pixel(im, j, i, 1, g);

            b = rgb[2] + diff;
            set_pixel(im, j, i, 2, b);
    }
    
    }
}