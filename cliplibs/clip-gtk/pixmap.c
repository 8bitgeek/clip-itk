/*
    Copyright (C) 2001  ITK
    Author  : Alexey M. Tkachenko <alexey@itk.ru>
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
#include "hashcode.h"
#include "clip.h"
#include "clip-gtkcfg.h"

#include <gtk/gtk.h>
#include <string.h>

#include "clip-gtk.ch"
#include "clip-gtk.h"
#include "bmpxpm.h"

#ifdef OS_CYGWIN
	#include "sys\cygwin.h"
#endif

#define MAXCOLORS       256

/*********************** SIGNALS **************************/

/****           Pixmap has no own signal               ****/

/**********************************************************/

/* Register pixmap in global table */
CLIP_DLLEXPORT GtkType _gtk_type_pixmap() { return GTK_TYPE_LABEL; }
long _clip_type_pixmap() { return GTK_WIDGET_LABEL; }
const char * _clip_type_name_pixmap() { return "GTK_WIDGET_LABEL"; }

int
clip_INIT___PIXMAP(ClipMachine *cm)
{
	_wtype_table_put(_clip_type_pixmap, _clip_type_name_pixmap, _gtk_type_pixmap, _gtk_type_misc, NULL);
	return 0;
}
/**********************************************************/

/**** PIXMAP constructor ****/
int
clip_GTK_PIXMAPNEW(ClipMachine * cm)
{
	ClipVar *cv = _clip_spar(cm, 1);
	C_widget *cwin   = _fetch_cwidget(cm,_clip_spar(cm, 2));
	gint  width = _clip_parni(cm, 3);
	gint height = _clip_parni(cm, 4);
	gint  depth = INT_OPTION(cm, 5, -1);
	GtkWidget *wid = NULL;
	C_widget *cwid;
//	GdkBitmap *mask;
	GdkPixmap *pixmap;
	GdkWindow *win = NULL;
	CHECKOPT(1,MAP_t);
	CHECKOPT2(2,MAP_t,NUMERIC_t); CHECKCWIDOPT(cwin,GTK_IS_WIDGET);
	CHECKOPT(3,NUMERIC_t); CHECKOPT(4,NUMERIC_t); CHECKOPT(5,NUMERIC_t);

	if (cwin && cwin->widget && cwin->widget->window)
	{
		win = cwin->widget->window;
	}
	pixmap = gdk_pixmap_new(win, width, height, depth);

//	if (pixmap) wid = gtk_pixmap_new(pixmap,mask);
	if (pixmap) wid = gtk_pixmap_new(pixmap,NULL);
	if (!wid) goto err;

	cwid = _register_widget(cm, wid, cv);
	cwid->data = pixmap;
	_clip_mclone(cm,RETPTR(cm),&cwid->obj);

	return 0;
err:
	return 1;
}
/**** ------------------ ****/

/**** PIXMAP constructor ****/
int
clip_GTK_PIXMAPFROMXPMNEW(ClipMachine * cm)
{
	ClipVar   * cv   = _clip_spar(cm, 1);
	char *filename   = _clip_parc(cm, 2);
	C_widget *cwin   = _fetch_cwidget(cm,_clip_spar(cm, 3));
	char buf[PATH_MAX];
	GtkWidget *wid = NULL;
	C_widget *cwid;
	GdkBitmap *mask;
	GdkPixmap *pixmap;
	GdkWindow *win = NULL;
	GdkColormap *colormap = NULL;
	CHECKOPT(1,MAP_t);
	CHECKOPT(2,CHARACTER_t);
	CHECKOPT2(3,MAP_t,NUMERIC_t); CHECKCWIDOPT(cwin,GTK_IS_WIDGET);

	LOCALE_TO_UTF(filename);
	#ifdef OS_CYGWIN
	cygwin_conv_to_win32_path(filename, buf);
	#else
	_clip_path(cm, filename, buf, sizeof(buf), 0 );
	#endif
	FREE_TEXT(filename);
	if (cwin && cwin->widget && cwin->widget->window)
	{
		win = cwin->widget->window;
		colormap = gtk_widget_get_colormap(cwin->widget);
	}
	else
		colormap = gdk_colormap_get_system();
//        pixmap = gdk_pixmap_create_from_xpm(win, &mask, NULL, buf);
	pixmap = gdk_pixmap_colormap_create_from_xpm(win, colormap,
		&mask, NULL, buf);
	if (pixmap) wid = gtk_pixmap_new(pixmap,mask);
	if (!wid) goto err;

	cwid = _register_widget(cm, wid, cv);
	cwid->data = pixmap;
	_clip_mclone(cm,RETPTR(cm),&cwid->obj);

	return 0;
err:
	return 1;
}
/**** ------------------ ****/


/**** PIXMAP from data constructor ****/
int
clip_GTK_PIXMAPCREATEFROMXPMD(ClipMachine * cm)
{
	ClipVar   * cv   = _clip_spar(cm, 1);
	ClipVar  * cxpmd = _clip_par(cm, 2);
	C_widget *cwin   = _fetch_cwidget(cm,_clip_spar(cm, 3));
	GtkWidget *wid = NULL;
	C_widget *cwid;
	GdkBitmap *mask;
	GdkPixmap *pixmap;
	GdkWindow *win = NULL;
	GdkColormap *colormap = NULL;
	char * * xpm_data = NULL;
	unsigned short i;
	CHECKOPT(1,MAP_t);
	CHECKOPT(2,ARRAY_t);
	CHECKOPT2(3,MAP_t,NUMERIC_t); CHECKCWIDOPT(cwin,GTK_IS_WIDGET);

	if (cwin && cwin->widget && cwin->widget->window)
	{
		win = cwin->widget->window;
		colormap = gtk_widget_get_colormap(cwin->widget);
	}
	else
		colormap = gdk_colormap_get_system();
	//pixmap = gdk_pixmap_create_from_xpm(win, &mask, NULL, buf);

	if (cxpmd->a.count > 0)
	{
		ClipVar *item;
		xpm_data = (char * *)calloc(sizeof(char*),cxpmd->a.count);
		for (i = 0; i < cxpmd->a.count; i++)
		{
			item = cxpmd->a.items + i;
			if (item->t.type == CHARACTER_t)
			{
				xpm_data[i] = (char*)calloc(1,item->s.str.len+1);
				strcpy(xpm_data[i], item->s.str.buf);
			}
		}
	}

	pixmap = gdk_pixmap_colormap_create_from_xpm_d(win, colormap, &mask, NULL, xpm_data);
	if (pixmap) wid = gtk_pixmap_new(pixmap,mask);
	if (xpm_data)
	{
		for (i = 0; i < cxpmd->a.count; i++)
		{
			if (xpm_data[i])
				free(xpm_data[i]);
		}
		free(xpm_data);
	}

	if (!wid) goto err;

	cwid = _register_widget(cm, wid, cv);
	cwid->data = pixmap;
	_clip_mclone(cm,RETPTR(cm),&cwid->obj);

	return 0;
err:
	return 1;
}
/**** ------------------ ****/

/* forwards */
static gchar ** _load_xpm_from_bmp(ClipMachine *cm,char *filename, int *ncolors, unsigned long *rows, unsigned long *cols);
static void _free_xpm_data(char * * xpm_data, unsigned long ncolors, unsigned long rows);

/**** PIXMAP from BMP constructor ****/
int
clip_GTK_PIXMAPFROMBMPNEW(ClipMachine * cm)
{
	ClipVar   * cv   = _clip_spar(cm, 1);
	char *filename   = _clip_parc(cm, 2);
	C_widget *cwin   = _fetch_cwidget(cm,_clip_spar(cm, 3));
	char buf[PATH_MAX];
	GtkWidget *wid = NULL;
	C_widget *cwid;
	GdkBitmap *mask;
	GdkPixmap *pixmap;
	GdkWindow *win = NULL;
	GdkColormap *colormap = NULL;
	gchar * * xpmdata;
	int ncolors;
	unsigned long rows,cols;
	CHECKOPT(1,MAP_t);
	CHECKOPT(2,CHARACTER_t);
	CHECKOPT2(3,MAP_t,NUMERIC_t); CHECKCWIDOPT(cwin,GTK_IS_WIDGET);

	LOCALE_TO_UTF(filename);
	_clip_path(cm, filename, buf, sizeof(buf), 0 );
	FREE_TEXT(filename);
	if (cwin && cwin->widget && cwin->widget->window)
	{
		win = cwin->widget->window;
		colormap = gtk_widget_get_colormap(cwin->widget);
	}
	else
		colormap = gdk_colormap_get_system();
	xpmdata = _load_xpm_from_bmp(cm,buf, &ncolors, &rows, &cols);
	if (xpmdata)
		pixmap = gdk_pixmap_colormap_create_from_xpm_d(win,
			colormap, &mask, NULL, xpmdata);
	_free_xpm_data(xpmdata, ncolors, rows);
	wid = gtk_pixmap_new(pixmap,mask);
	if (!wid) goto err;

	cwid = _register_widget(cm, wid, cv);
	cwid->data = pixmap;
	_clip_mclone(cm,RETPTR(cm),&cwid->obj);

	return 0;
err:
	return 1;
}
/**** ------------------ ****/

/* build : set to TRUE if an extra pixmap should be automatically
 * created to use when the pixmap is insensitive.  */
int
clip_GTK_PIXMAPSETBUILDINSENSITIVE(ClipMachine * cm)
{
	C_widget *cpxm = _fetch_cw_arg(cm);
	gboolean build = _clip_parl(cm, 2);
	CHECKCWID(cpxm,GTK_IS_PIXMAP);
	CHECKOPT(2,LOGICAL_t);
	if (_clip_parinfo(cm,2)==UNDEF_t) build = TRUE;
	gtk_pixmap_set_build_insensitive(GTK_PIXMAP(cpxm->widget), build);
	return 0;
err:
	return 1;
}

static gchar **
_load_xpm_from_bmp(ClipMachine *cm,char *filename, int *colors, unsigned long *nrows, unsigned long *ncols)
{
	FILE           *ifp;

	int             rc;
	unsigned long   pos = 0;
	unsigned long   rows, cols, pixstrlen;

	unsigned long   offBits;

	unsigned short  cBitCount;
	int             class;

	pixval          R[MAXCOLORS];   /* reds */
	pixval          G[MAXCOLORS];   /* greens */
	pixval          B[MAXCOLORS];   /* blues */

	lpixel        **lpixels;
	char          **xpm_data, **pxpm, *ppixstr;
	char          buf[300];
	register lpixel *pP;

	int ncolors, row, col, i;
	pixval maxval;                  /* pixval == unsigned char or
					 * unsigned short */
	colorhash_table cht;
	colorhist_vector chv;
	char err[100];
	/* Used for rgb value -> character-pixel string mapping */
	cixel_map *cmap;    /* cixel_map cmap[MAXCOLORS]; */
	int charspp;                    /* chars per pixel */

	if (!(ifp = fopen(filename, "rb")))
	{
		sprintf(err,"can't read file %s\n",filename);
		_clip_trap_err(cm,EG_READ,FALSE,FALSE,"CLIP_GTK_SYSTEM",EG_READ,err);
		return NULL;
	}

	BMPreadfileheader(ifp, &pos, &offBits);
	BMPreadinfoheader(ifp, &pos, &cols, &rows, &cBitCount, &class);
	*nrows = rows; *ncols = cols;

	if(offBits != BMPoffbits(class, cBitCount))
	{
		sprintf(err,"warning: offBits is %ld, expected %ld",
			pos, BMPoffbits(class, cBitCount));
		_clip_trap_err(cm,EG_READ,FALSE,FALSE,"CLIP_GTK_SYSTEM",EG_READ,err);
		return NULL;
	}

	rc = BMPreadrgbtable(ifp, &pos, cBitCount, class, R, G, B);

	if(rc != BMPlenrgbtable(class, cBitCount))
	{
		sprintf(err,"warning: %d-byte RGB table, expected %ld bytes",
			rc, BMPlenrgbtable(class, cBitCount));
		_clip_trap_err(cm,EG_READ,FALSE,FALSE,"CLIP_GTK_SYSTEM",EG_READ,err);
		return NULL;
	}


	lpixels = BMPreadbits(ifp, &pos, offBits, cols, rows
			, cBitCount, class, R, G, B);

	if(pos != BMPlenfile(class, cBitCount, cols, rows))
	{
		sprintf(err,"warning: read %ld bytes, expected to read %ld bytes",
			pos, BMPlenfile(class, cBitCount, cols, rows));
		_clip_trap_err(cm,EG_READ,FALSE,FALSE,"CLIP_GTK_SYSTEM",EG_READ,err);
		return NULL;
	}

	fclose(ifp);

	/**********************************************/
	/*
	* "maxval" is the largest value that can be be found in the ppm file.
	* All pixel components are relative to this value.
	*/
	maxval = (1 << (cBitCount))-1;
	/**********************************************/
	/* Figure out the colormap. */

	chv = ppm_computecolorhist(lpixels, cols, rows, MAXCOLORS, &ncolors);
	*colors = ncolors;
	if (chv == (colorhist_vector) 0)
	{
		sprintf(err,"too many colors - try running the pixmap through 'ppmquant 256'");
		_clip_trap_err(cm,EG_READ,FALSE,FALSE,"CLIP_GTK_SYSTEM",EG_READ,err);
		return NULL;
	}

	/* Make a hash table for fast color lookup. */
	cht = ppm_colorhisttocolorhash(chv, ncolors);

	cmap = (cixel_map *)malloc(ncolors * sizeof(cixel_map));
	if (cmap == (cixel_map *) NULL)
	{
		_clip_trap_err(cm,EG_MEM,FALSE,FALSE,"CLIP_GTK_SYSTEM",EG_MEM,"out of memory");
		return NULL;
	}
	/* Now generate the character-pixel colormap table. */
	gen_cmap(chv, ncolors, maxval, 0, NULL, 0,
		cmap, &charspp);

	/* Write out the XPM table. */
	xpm_data = (char**)calloc(ncolors+rows+1,sizeof(char*));
	pxpm = xpm_data;

	/* XPM */
	/* static char *xpm_data[] = { */
	/* width height ncolors chars_per_pixel */
	sprintf(buf,"%ld %ld %d %d", cols, rows, ncolors, charspp);
	*pxpm = strdup(buf); pxpm++;
	/* colors */
	for (i = 0; i < ncolors; i++) {
		sprintf(buf,"%s c %s", cmap[i].cixel, cmap[i].rgbname);
		*pxpm = strdup(buf); pxpm++;
	}
	/* pixels */

	pixstrlen = charspp * cols + 1;
	for (row = 0; row < rows; row++)
	{
		*pxpm = (char*)calloc(1,pixstrlen);
		ppixstr = *pxpm;
		for (col = 0, pP = lpixels[row]; col < cols; col++, pP++)
		{
			memcpy(ppixstr, cmap[ppm_lookupcolor(cht, pP)].cixel, charspp);
			ppixstr += charspp;
		}
		pxpm++;
	}
	ppm_freearray( lpixels, rows );
	ppm_freecolorhash(cht);
	free(cmap);

	return xpm_data;
}

static void
_free_xpm_data(char * * xpm_data, unsigned long ncolors, unsigned long rows)
{
	unsigned long i;
	if (!xpm_data) return;
	for (i = 0; i < ncolors+rows+1; i++)
		if (xpm_data[i])
			free(xpm_data[i]);
	free(xpm_data);
}








