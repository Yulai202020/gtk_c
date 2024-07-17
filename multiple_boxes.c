#include <gtk/gtk.h>

// this app will do nothink
int main(int argc, char** argv) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // make window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // setting of window
    gtk_window_set_title(GTK_WINDOW(window), "My title"); // title
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400); // size
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // position (where is mouse)
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE); // resizable is false
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // if window was closed exit program

    // main_box will has every boxes
    GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget* box1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(main_box), entry_box, FALSE, FALSE, 0);

    GtkWidget* box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(main_box), entry_box, FALSE, FALSE, 0);

    // show window
    gtk_widget_show_all(window);

    // run
    gtk_main();
    return 0;
}
