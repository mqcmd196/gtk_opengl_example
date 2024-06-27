#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <epoxy/egl.h>

// OpenGL初期化関数
void realize_cb(GtkGLArea *area) {
    gtk_gl_area_make_current(area);

    if (gtk_gl_area_get_error(area) != NULL) {
        return;
    }

    glEnable(GL_DEPTH_TEST);
}

// OpenGL描画関数
gboolean render_cb(GtkGLArea *area, GdkGLContext *context) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0.0, 0.5);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(-0.5, -0.5);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0.5, -0.5);
    glEnd();

    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK OpenGL Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    GtkWidget *gl_area = gtk_gl_area_new();
    gtk_container_add(GTK_CONTAINER(window), gl_area);

    g_signal_connect(gl_area, "realize", G_CALLBACK(realize_cb), NULL);
    g_signal_connect(gl_area, "render", G_CALLBACK(render_cb), NULL);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

