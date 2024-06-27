//
// Created by Yoshiki Obinata on 2024/06/25.
//
#include <gtk/gtk.h>
#include <epoxy/gl.h>

static void realize(GtkGLArea *area, gpointer data) {
    gtk_gl_area_make_current(area);
    if (gtk_gl_area_get_error(area) != NULL) {
        return;
    }

    // OpenGLの初期設定
    glClearColor(0, 0, 0, 1);
}

static void render(GtkGLArea *area, GdkGLContext *context) {
    gtk_gl_area_make_current(area);
    if (gtk_gl_area_get_error(area) != NULL) {
        return;
    }

    // 画面クリア
    glClear(GL_COLOR_BUFFER_BIT);

    // 赤い四角形を描画
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f( 0.5f, -0.5f);
    glVertex2f( 0.5f,  0.5f);
    glVertex2f(-0.5f,  0.5f);
    glEnd();

    gtk_gl_area_queue_render(area);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *gl_area = gtk_gl_area_new();
    gtk_container_add(GTK_CONTAINER(window), gl_area);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(gl_area, "realize", G_CALLBACK(realize), NULL);
    g_signal_connect(gl_area, "render", G_CALLBACK(render), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
