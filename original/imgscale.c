/*
 * save as "imgscale.c", compile with:
 * $ gcc -shared -o imgscale.so -I/usr/include/tcl8.5 imgscale.c -DUSE_TCL_STUBS -DUSE_TK_STUBS -ltclstub8.5 -ltkstub8.5
 */
/*
** Scale an image using grid sampling
** Please note this implementation will crash if the
** destination image was not already created 
**
** Arguments: srcimg newwidth newheight destimg alpha
** srcimg: A photo object created with "image create photo"
** newwidth: The new width of the image in pixels
** newheight: The new height of the image in pixels
** destimage: Name for the resulting image
** alpha: Alpha to apply to the image (1 - opaque, 0-clear)
*/

#include <tcl.h>
#include <tk.h> 

static int irmScaleImgCmd(
  void *pArg,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
    char *srcName, *destName;
    Tk_PhotoImageBlock srcBlock, destBlock;
    Tk_PhotoHandle srcImage, destImage;
    int di, dj;
    double scalex, scaley, sx2, sy2, newalpha;
    int returnCode;
    int width, height, newwid, newhgt;

    if (objc != 5 && objc != 6) {
        Tcl_WrongNumArgs(interp, 1, objv, "srcimg newwid newhgt destimg ?alpha?");
        return TCL_ERROR;
    }
    srcName = Tcl_GetString(objv[1]);
    if (Tcl_GetIntFromObj(interp, objv[2], &newwid))
        return TCL_ERROR;
    if (Tcl_GetIntFromObj(interp, objv[3], &newhgt))
        return TCL_ERROR;
    destName=Tcl_GetString(objv[4]);
    if (objc == 6) {
        if (Tcl_GetDoubleFromObj(interp, objv[5], &newalpha))
            return TCL_ERROR;
    } else {
        newalpha=1.0;
    }
    if (newalpha>1.0) {
        newalpha=1.0;
    }

    srcImage = Tk_FindPhoto(interp, srcName);
    if (!srcImage)
        return TCL_ERROR;
    Tk_PhotoGetSize(srcImage, &width, &height);
    Tk_PhotoGetImage(srcImage, &srcBlock);

    if (!srcBlock.pixelPtr) {
        /* Empty image. Do nothing */
        return TCL_OK;
    }

    if (srcBlock.pixelSize != 4 && srcBlock.pixelSize!=3) {
        Tcl_AppendResult(interp, "I can't make heads or tails from this image, the bitfield is neither 3 nor 4", NULL);
        return TCL_ERROR;
    }

    destImage = Tk_FindPhoto(interp, destName);
    if (!destImage)
        return TCL_ERROR;
    Tk_PhotoBlank(destImage);
    Tk_PhotoSetSize(interp, destImage, newwid, newhgt);    

    destBlock.width = newwid;
    destBlock.height = newhgt;
    
    scalex = srcBlock.width / (double) newwid;
    scaley = srcBlock.height / (double) newhgt;
    sx2 = scalex / 2.0;
    sy2 = scaley / 2.0;

    destBlock.pixelSize = 4;
    destBlock.pitch = newwid * 4;
    destBlock.offset[0] = 0; 
    destBlock.offset[1] = 1; 
    destBlock.offset[2] = 2; 
    destBlock.offset[3] = 3;
    destBlock.pixelPtr = (unsigned char *) Tcl_Alloc(destBlock.width * destBlock.height * 4);

    /* Loop through and scale */
    for (dj=0 ; dj<destBlock.height ; dj++) {
        for (di=0 ; di<destBlock.width ; di++) {
            int si, sj;
            int cx = (int)(di * scalex);
            int cy = (int)(dj * scaley);
            int points = 1;
            double red = 0.0, green = 0.0, blue = 0.0, alpha = 0.0;
            int newoff = destBlock.pitch*dj + destBlock.pixelSize*di;
            int startoff = srcBlock.pitch*cy + srcBlock.pixelSize*cx;

            red = (double) srcBlock.pixelPtr[startoff + srcBlock.offset[0]];
            green = (double) srcBlock.pixelPtr[startoff + srcBlock.offset[1]];
            blue = (double) srcBlock.pixelPtr[startoff + srcBlock.offset[2]];
            if (srcBlock.pixelSize == 4) {
                alpha = (double) srcBlock.pixelPtr[startoff + srcBlock.offset[3]];
            } else {
                alpha += 255;
            }
            for (sj=(int)cy-sy2 ; sj<(int)cy+sy2 ; sj++) {
                if (sj < 0)
                    continue;
                if (sj > srcBlock.height)
                    continue;
                for (si=(int)cx-sx2 ; si<(int)cx+sx2 ; si++) {
                    int offset = srcBlock.pitch*sj + srcBlock.pixelSize*si;
                    if (si < 0)
                        continue;
                    if (si > srcBlock.width)
                        continue;

                    points++;
                    red += (double) srcBlock.pixelPtr[offset + srcBlock.offset[0]];
                    green += (double) srcBlock.pixelPtr[offset + srcBlock.offset[1]];
                    blue += (double) srcBlock.pixelPtr[offset + srcBlock.offset[2]];
                    if (srcBlock.pixelSize == 4) {
                        alpha += (double) srcBlock.pixelPtr[offset + srcBlock.offset[3]];
                    } else {
                        alpha += 255;
                    }
                }
            }
            destBlock.pixelPtr[newoff + destBlock.offset[0]] = (unsigned char)(red / points);
            destBlock.pixelPtr[newoff + destBlock.offset[1]] = (unsigned char)(green / points);
            destBlock.pixelPtr[newoff + destBlock.offset[2]] = (unsigned char)(blue / points);
            destBlock.pixelPtr[newoff + destBlock.offset[3]] = (unsigned char)(alpha*newalpha / points);
        }
    }
    returnCode = Tk_PhotoPutBlock(interp, destImage, &destBlock, 0, 0,
            destBlock.width, destBlock.height, TK_PHOTO_COMPOSITE_SET);
    return returnCode;
}


/*
** The following is the only public symbol in this source file.  .
*/
int DLLEXPORT Imgscale_Init(Tcl_Interp *interp){
    if(Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
        return TCL_ERROR;
    }
    if(Tk_InitStubs(interp, TK_VERSION, 0) == NULL) {
        return TCL_ERROR;
    }
    Tcl_CreateObjCommand(interp, "image_scale", irmScaleImgCmd, 0, 0);
    return TCL_OK;
}
