#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <opencv2/opencv.hpp>

using namespace cv;

static GLuint texture = 0;
static int win_width = 800;
static int win_height = 640;

// OpenGLの初期設定
static void realize(GtkGLArea *area, gpointer user_data) {
    gtk_gl_area_make_current(area);
    if (gtk_gl_area_get_error(area) != NULL)
        return;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// 画像のテクスチャを更新
void load_texture_from_image(const char* filename) {
    std::cout << "Loading image: " << filename << std::endl;
    Mat img = imread(filename);
    if (img.empty()) {
        std::cerr << "Failed to load image." << std::endl;
        return;
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.data);
}

// 描画コールバック
static gboolean render(GtkGLArea *area, GdkGLContext *context) {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-1, -1);
    glTexCoord2f(1, 0); glVertex2f(1, -1);
    glTexCoord2f(1, 1); glVertex2f(1, 1);
    glTexCoord2f(0, 1); glVertex2f(-1, 1);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *gl_area = gtk_gl_area_new();
    gtk_widget_set_size_request(gl_area, win_width, win_height);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(gl_area, "realize", G_CALLBACK(realize), NULL);
    g_signal_connect(gl_area, "render", G_CALLBACK(render), NULL);

    gtk_container_add(GTK_CONTAINER(window), gl_area);
    gtk_widget_show_all(window);

    if (argc > 1) {
        load_texture_from_image(argv[1]);
    } else {
        load_texture_from_image(SAMPLE_IMAGE_PATH);
    }

    gtk_main();

    return 0;
}
