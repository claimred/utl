#ifndef _BUNDLE_HELPER_H_
#define _BUNDLE_HELPER_H_

#include <iostream>

#include <CoreFoundation/CFBundle.h>

namespace utl_core
{
   static CFBundleRef _mainBundle;

   class BundleHelper
   {
   public:
     
     //Singleton class. Not thread-safe
     static BundleHelper &getInstance()
     {
         static BundleHelper bh;
         return bh;
     }
     
     static char *getPath( const char *filename )
     {
         CFURLRef resourceURL;
         
         resourceURL = CFBundleCopyResourceURL(_mainBundle, 
            CFStringCreateWithCString(NULL, filename, kCFStringEncodingASCII),NULL, NULL);
         
         if (!resourceURL)
             throw "Failed to locate a file in the loaded bundle!";
         
         char *fileurl = new char [200];
         
         
         if (!CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8*)fileurl, 200))
             throw "Failed to turn a bundle resource URL into a filesystem path representation!";
         else
             return fileurl;
     }
     
   private:
     
     BundleHelper() { _mainBundle = CFBundleGetMainBundle(); };
     BundleHelper( BundleHelper & );
     void operator=( BundleHelper& );
     
   };
}

#endif /* _BUNDLE_HELPER_H_ */
