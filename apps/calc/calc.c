#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "string.c"

GtkWidget* entry;

void skipWhitespace(const char** expr) {
    while (isspace(**expr)) {
        (*expr)++;
    }
}

double evaluateNumber(const char** expr) {
    double num = 0.0;
    while (isdigit(**expr) || **expr == '.') {
        num = num * 10.0 + (**expr - '0');
        (*expr)++;
    }
    
    if (**expr == '/') {
        (*expr)++;
        double denominator = evaluateNumber(expr);
        if (denominator != 0) {
            return num / denominator;
        } else {
            fprintf(stderr, "Error: Division by zero\n");
            exit(EXIT_FAILURE);
        }
    }
    
    return num;
}

double evaluateSubExpression(const char** expr) {
    double result = 0.0;
    double current_num = 0.0;
    char operation = '+';
    
    while (**expr) {
        skipWhitespace(expr);
        
        if (isdigit(**expr)) {
            current_num = evaluateNumber(expr);
        } else if (**expr == '(') {
            (*expr)++; // Move past '('
            current_num = evaluateSubExpression(expr);
        } else if (**expr == '+' || **expr == '-' || **expr == '*' || **expr == '/') {
            if (operation == '+') {
                result += current_num;
            } else if (operation == '-') {
                result -= current_num;
            } else if (operation == '*') {
                result *= current_num;
            } else if (operation == '/') {
                if (current_num != 0.0) {
                    result /= current_num;
                } else {
                    fprintf(stderr, "Error: Division by zero\n");
                    exit(EXIT_FAILURE);
                }
            }
            operation = **expr;
            (*expr)++;
        } else if (**expr == ')') {
            (*expr)++; // Move past ')'
            break; // End of sub-expression
        } else {
            // Handle unexpected characters or syntax errors
            break;
        }
    }
    
    // Final operation with the last number in the sub-expression
    if (operation == '+') {
        result += current_num;
    } else if (operation == '-') {
        result -= current_num;
    } else if (operation == '*') {
        result *= current_num;
    } else if (operation == '/') {
        if (current_num != 0.0) {
            result /= current_num;
        } else {
            fprintf(stderr, "Error: Division by zero\n");
            exit(EXIT_FAILURE);
        }
    }
    
    return result;
}

double evaluateExpression(const char* expr) {
    return evaluateSubExpression(&expr);
}

char* double_to_string(double number) {
    // Allocate memory for the string representation
    char* buffer = (char*)malloc(20 * sizeof(char)); // Assuming sufficient size for the number
    
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
