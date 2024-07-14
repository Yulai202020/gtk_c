#include <gtk/gtk.h>

static void on_button_clicked(GtkWidget* widget, gpointer data) {
    const gchar* label = gtk_button_get_label(GTK_BUTTON(widget)); // gchar is same think with a char

    g_print("Button with label '");
    g_print(label);
    g_print("' was pressed\n"); // g_print print while program is started
}

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

    // make box
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    // add to window box
    gtk_container_add(GTK_CONTAINER(window), box);

    // make button
    GtkWidget* button = gtk_button_new_with_label("button label");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // add button to box
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    // show window
    gtk_widget_show_all(window);

    // run
    gtk_main();
    return 0;
}
