#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
void load_image(char* filename, GtkWidget* widget) {
    FILE* infile;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPARRAY buffer;
    int row_stride;
    unsigned char* data;
    unsigned char* p;
    int i, j;
    int width, height;

    if ((infile = fopen(filename, "rb")) == NULL) {
        printf("Unable to open file %s\n", filename);
        exit(1);
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    (void) jpeg_read_header(&cinfo, TRUE);
    (void) jpeg_start_decompress(&cinfo);

    width = cinfo.output_width;
    height = cinfo.output_height;
    row_stride = width * cinfo.output_components;

    data = (unsigned char*) malloc(width * height * cinfo.output_components);
    buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    p = data;
    while (cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        for (j = 0; j < row_stride; j++) {
            *(p++) = buffer[0][j];
        }
    }

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(infile);

    GdkPixbuf* pixbuf = gdk_pixbuf_new_from_data(data, GDK_COLORSPACE_RGB, FALSE, 8, width, height, row_stride, NULL, NULL);

    gtk_image_set_from_pixbuf(GTK_IMAGE(widget), pixbuf);

    g_object_unref(pixbuf);
    free(data);
}
int main(int argc, char* argv[]) {
    GtkWidget* window;
    GtkWidget* image;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Image Viewer");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    image = gtk_image_new();

    load_image(argv[1], image);

    gtk_container_add(GTK_CONTAINER(window), image);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
