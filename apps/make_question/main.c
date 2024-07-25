#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "string.c"

GtkWidget* ans;

static void on_button_clicked(GtkWidget* widget, gpointer data) {
    const gchar* text = gtk_entry_get_text(GTK_ENTRY(ans));
    printf("%s\n", text);
    gtk_entry_set_text(GTK_ENTRY(ans), "");
}

int main(int argc, char** argv) {
    char* full_message = "";

    for (int i = 1; i < argc; i++) {
        full_message = stringSum(full_message, argv[i]);
        if (i < argc - 1) {
            full_message = stringSum(full_message, " ");
        }
    }

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Idk");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box container as the main container for the window
    GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Create an entry box and add it to the main box
    GtkWidget* label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget* label = gtk_label_new(full_message);
    gtk_box_pack_start(GTK_BOX(label_box), label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), label_box, FALSE, FALSE, 0);

    GtkWidget* ans_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget* label2 = gtk_label_new("Answer:");
    ans = gtk_entry_new();
    GtkWidget* submit = gtk_button_new_with_label("Submit");
    g_signal_connect(submit, "clicked", G_CALLBACK(on_button_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(ans_box), label2, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(ans_box), ans, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(ans_box), submit, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), ans_box, FALSE, FALSE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Run the GTK main loop
    gtk_main();
    return 0;
}