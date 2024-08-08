#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <yulai/string.h> // its my library you can install it from my git https://github.com/Yulai202020/c_course in dir: my_libs/string.c

GtkWidget* entry;

#define MAX_EXPR_LEN 1024

char available_items[] = "0123456789+-*/()";

int precedence(char op) {
    if(op == '+' || op == '-')
        return 1;
    if(op == '*' || op == '/')
        return 2;
    return 0;
}

double applyOperation(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

double evaluateExpression(const char* expression) {
    double values[MAX_EXPR_LEN];
    char ops[MAX_EXPR_LEN];
    int valTop = -1, opsTop = -1;

    for(int i = 0; expression[i]; i++) {
        if(expression[i] == ' ')
            continue;

        if(isdigit(expression[i]) || expression[i] == '.') {
            char buffer[16];
            int bufIndex = 0;

            while(i < strlen(expression) && (isdigit(expression[i]) || expression[i] == '.')) {
                buffer[bufIndex++] = expression[i++];
            }
            buffer[bufIndex] = '\0';
            i--;

            values[++valTop] = atof(buffer);
        } else if(expression[i] == '(') {
            ops[++opsTop] = expression[i];
        } else if(expression[i] == ')') {
            while(opsTop != -1 && ops[opsTop] != '(') {
                double val2 = values[valTop--];
                double val1 = values[valTop--];
                char op = ops[opsTop--];
                values[++valTop] = applyOperation(val1, val2, op);
            }
            opsTop--; 
        } else {
            while(opsTop != -1 && precedence(ops[opsTop]) >= precedence(expression[i])) {
                double val2 = values[valTop--];
                double val1 = values[valTop--];
                char op = ops[opsTop--];
                values[++valTop] = applyOperation(val1, val2, op);
            }
            ops[++opsTop] = expression[i];
        }
    }

    while(opsTop != -1) {
        double val2 = values[valTop--];
        double val1 = values[valTop--];
        char op = ops[opsTop--];
        values[++valTop] = applyOperation(val1, val2, op);
    }

    return values[valTop];
}

char* double_to_string(double number) {
    // Allocate memory for the string representation
    char* buffer = (char*) malloc(20 * sizeof(char)); // Assuming sufficient size for the number
    
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1); // Exit if malloc fails
    }

    // Convert double to string
    sprintf(buffer, "%lf", number);

    return buffer;
}

static void on_button_clicked(GtkWidget* widget, gpointer data) {
    const gchar* label = gtk_button_get_label(GTK_BUTTON(widget));
    const gchar* text = gtk_entry_get_text(GTK_ENTRY(entry));

    if (are_equal(label, "AC")) {
        gtk_entry_set_text(GTK_ENTRY(entry), "");
    } else if (are_equal(label, "=")) {
        double result = evaluateExpression(text);
        char* result_str = double_to_string(result);
        gtk_entry_set_text(GTK_ENTRY(entry), result_str);
        // int is_good = 1;
        // for (int i = 0; expression[i]; i++) {
        //     if (strchr(available_items, expression[i]) == NULL && !isspace(expression[i])) {
        //         printf("Invalid symbol.\n");
        //         is_good = 0;
        //         break;
        //     }
        // }

        // if (!is_good || strlen(expression) == 0) {
        //     free(expression);
        //     continue;
        // }
        free(result_str);
    } else {
        char* new_text = stringSum(text, label);
        gtk_entry_set_text(GTK_ENTRY(entry), new_text);
        free(new_text);
    }
}

void add_buttons(GtkWidget* box, char* list_to_add[], size_t len) {
    for (int i = 0; i < len; i++) {
        char* label = list_to_add[i];
        GtkWidget* button = gtk_button_new_with_label(label);
        g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
        gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);
    }
}

int main(int argc, char** argv) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box container as the main container for the window
    GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Create an entry box and add it to the main box
    GtkWidget* entry_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(entry_box), entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), entry_box, FALSE, FALSE, 0);

    GtkWidget* special_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    char* lists[] = {"(", ")", " ", "AC"};
    add_buttons(special_row, lists, 4);
    gtk_box_pack_start(GTK_BOX(main_box), special_row, TRUE, TRUE, 0);

    // Create a box for buttons and add buttons to it
    GtkWidget* first_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    char* list1[] = {"1", "2", "3", "/"};
    add_buttons(first_row, list1, 4);
    gtk_box_pack_start(GTK_BOX(main_box), first_row, TRUE, TRUE, 0);

    GtkWidget* second_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    char* list2[] = {"4", "5", "6", "*"};
    add_buttons(second_row, list2, 4);
    gtk_box_pack_start(GTK_BOX(main_box), second_row, TRUE, TRUE, 0);

    GtkWidget* third_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    char* list3[] = {"7", "8", "9", "-"};
    add_buttons(third_row, list3, 4);
    gtk_box_pack_start(GTK_BOX(main_box), third_row, TRUE, TRUE, 0);

    GtkWidget* fourth_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    char* list4[] = {"0", ".", "=", "+"};
    add_buttons(fourth_row, list4, 4);
    gtk_box_pack_start(GTK_BOX(main_box), fourth_row, TRUE, TRUE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Run the GTK main loop
    gtk_main();

    return 0;
}
