extern "C"
{
   #include <X11/Xlib.h>
   #include <X11/Xutil.h>
   #include <GL/gl.h>
   #include <GL/glx.h>
}

#include <iostream>
#include <string>


/**
 * Choose a simple FB Config.
 */
static GLXFBConfig *
choose_fb_config(Display *dpy, int scrnum)
{
   int fbAttribSingle[] = {
      GLX_RENDER_TYPE,   GLX_RGBA_BIT,
      GLX_RED_SIZE,      1,
      GLX_GREEN_SIZE,    1,
      GLX_BLUE_SIZE,     1,
      GLX_DOUBLEBUFFER,  False,
      None };
   int fbAttribDouble[] = {
      GLX_RENDER_TYPE,   GLX_RGBA_BIT,
      GLX_RED_SIZE,      1,
      GLX_GREEN_SIZE,    1,
      GLX_BLUE_SIZE,     1,
      GLX_DOUBLEBUFFER,  True,
      None };
   GLXFBConfig *configs;
   int nConfigs;

   configs = glXChooseFBConfig(dpy, scrnum, fbAttribSingle, &nConfigs);
   if (!configs)
      configs = glXChooseFBConfig(dpy, scrnum, fbAttribDouble, &nConfigs);

   return configs;
}


std::string get_screen_info(Display *dpy, int scrnum)
{
   Window win;
   XSetWindowAttributes attr;
   unsigned long mask;
   Window root;
   GLXContext ctx = NULL;
   XVisualInfo *visinfo;
   int width = 100, height = 100;
   GLXFBConfig *fbconfigs;

   root = RootWindow(dpy, scrnum);

   /*
    * Choose FBConfig or XVisualInfo and create a context.
    */
   fbconfigs = choose_fb_config(dpy, scrnum);
   if (fbconfigs) {
      ctx = glXCreateNewContext(dpy, fbconfigs[0], GLX_RGBA_TYPE, NULL, GL_TRUE);
      visinfo = glXGetVisualFromFBConfig(dpy, fbconfigs[0]);
      XFree(fbconfigs);
   } else
      visinfo = NULL;

   if (!ctx) {
      XFree(visinfo);
      return False;
   }

   /*
    * Create a window so that we can just bind the context.
    */
   attr.background_pixel = 0;
   attr.border_pixel = 0;
   attr.colormap = XCreateColormap(dpy, root, visinfo->visual, AllocNone);
   attr.event_mask = StructureNotifyMask | ExposureMask;
   mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;
   win = XCreateWindow(dpy, root, 0, 0, width, height,
		       0, visinfo->depth, InputOutput,
		       visinfo->visual, mask, &attr);

   std::string version_string = "";

   if (glXMakeCurrent(dpy, win, ctx)) {
      const char *glVersion = (const char *) glGetString(GL_VERSION);
      version_string = glVersion;
   }
   else {
     version_string = "";
   }

   glXDestroyContext(dpy, ctx);
   XFree(visinfo);
   XDestroyWindow(dpy, win);
   XSync(dpy, 1);

   return version_string;
}


std::string get_opengl_info() {

   Display *dpy;
   int numScreens;

   std::string version_string = "";

   dpy = XOpenDisplay(NULL);
   if (!dpy) {

      std::cout << "Error: unable to open display" << XDisplayName(NULL) << std::endl;
      version_string = "";

   } else {

      numScreens = ScreenCount(dpy);

      if (numScreens == 0)
         std::cout << "Error: 0 displays opened." << std::endl;
      else
         version_string = get_screen_info(dpy, 0);

      XCloseDisplay(dpy);

   }

   return version_string;
}
