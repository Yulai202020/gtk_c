#include <gtk/gtk.h>
#include <glib.h>

#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include <yulai/string.h> // its my library you can install it from my git https://github.com/Yulai202020/c_course in dir: my_libs/string.c

GtkWidget* label;
GtkWidget* ans;

static void initialize_readline() {
    rl_catch_signals = 0; // Let GTK handle signals
}

static void on_button_clicked(GtkWidget* widget, gpointer data) {
    const gchar* text = gtk_entry_get_text(GTK_ENTRY(ans));
    printf("Answer: %s\n", text);
    gtk_entry_set_text(GTK_ENTRY(ans), "");
}

static gboolean readline_callback(GIOChannel *source, GIOCondition condition, gpointer data) {
    if (condition & G_IO_IN) {
        char *input = readline(NULL);
        if (input && *input) {
            add_history(input);
            gtk_label_set_text(GTK_LABEL(label), input);
            free(input);
        }
    }
    return TRUE;
}

static void integrate_readline_with_gtk() {
    int fd = fileno(stdin);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK); // Set stdin to non-blocking mode
    GIOChannel *channel = g_io_channel_unix_new(fd);
    g_io_add_watch(channel, G_IO_IN | G_IO_ERR | G_IO_HUP, readline_callback, NULL);
}

int main(int argc, char** argv) {
    char* full_message = sjoin(argv, 1, argc, " ");

    // Initialize GTK
    gtk_init(&argc, &argv);

    initialize_readline();
    integrate_readline_with_gtk();

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
    label = gtk_label_new(full_message);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
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

    gtk_main();
    return 0;
}
