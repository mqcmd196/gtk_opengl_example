#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <iostream>
#include <functional>
#include <opencv2/opencv.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLWindow {
public:
    GtkWidget *glarea;
    GLWindow(GtkWidget* window) {
        gtk_window_set_title(GTK_WINDOW(window), "GTK3 + OpenGL Example");
        gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
        glarea = gtk_gl_area_new();
        gtk_container_add(GTK_CONTAINER(window), glarea);

        g_signal_connect(glarea, "realize", G_CALLBACK(on_realize), this);
        g_signal_connect(glarea, "render", G_CALLBACK(on_render), this);
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    }

    void realize(GtkGLArea *area) {
        gtk_gl_area_make_current(area);
        if (gtk_gl_area_get_error(area) != NULL)
            return;

        if (init_callback) {
            init_callback();
        }
    }

    gboolean render(GtkGLArea *area, GdkGLContext *context) {
        gtk_gl_area_make_current(area);
        if (gtk_gl_area_get_error(area) != NULL)
            return FALSE;
        gtk_widget_queue_draw(glarea);
        if (render_callback) {
            render_callback();
        }

        return TRUE;
    }

    void setGlCallback(std::function<void()> callback) {
        render_callback = callback;
    }

    void setInitCallback(std::function<void()> callback) {
        init_callback = callback;
    }

private:
    std::function<void()> render_callback;
    std::function<void()> init_callback;

    static void on_realize(GtkGLArea *area, gpointer user_data) {
        GLWindow *glwindow = static_cast<GLWindow*>(user_data);
        glwindow->realize(area);
    }

    static gboolean on_render(GtkGLArea *area, GdkGLContext *context, gpointer user_data) {
        GLWindow *glwindow = static_cast<GLWindow*>(user_data);
        return glwindow->render(area, context);
    }
};

static GLuint create_shader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    return shader;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    GLWindow glwindow(window);

    GLuint vao, vbo, program, textureID;
//    load image
    cv::Mat image = cv::imread(SAMPLE_IMAGE_PATH);
    cv::flip(image, image, 0);

    glwindow.setInitCallback([&]() {
        glEnable(GL_DEPTH_TEST);
        const char *vertex_shader_source =
                "#version 330 core\n"
                "layout (location = 0) in vec3 position;\n"
                "layout (location = 1) in vec2 texCoord;\n"
                "out vec2 TexCoord;\n"
                "uniform mat4 transform;\n"
                "void main() {\n"
                "   gl_Position = transform * vec4(position, 1.0);\n"
                "   TexCoord = texCoord;\n"
                "}\n";
        const char *fragment_shader_source =
                "#version 330 core\n"
                "in vec2 TexCoord;\n"
                "out vec4 color;\n"
                "uniform sampler2D ourTexture;\n"
                "void main() {\n"
                "   color = texture(ourTexture, TexCoord);\n"
                "}\n";
        program = glCreateProgram();
        GLuint vertex_shader = create_shader(vertex_shader_source, GL_VERTEX_SHADER);
        GLuint fragment_shader = create_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        glUseProgram(program);

        GLfloat vertices[] = {
                // Positions        // Texture Coords
                1.0f,  1.0f, 0.0f,  1.0f, 1.0f,   // Top Right
                1.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // Bottom Right
                -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,   // Top Left
                -1.0f, -1.0f, 0.0f,  0.0f, 0.0f    // Bottom Left
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Texture Coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0); // Unbind VAO


//        Image to texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    });

    glwindow.setGlCallback([&]() {
        static float angle = 0.0f;
        angle += 1.f;

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Y軸周りに回転

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    });

    gtk_widget_show(window);
    gtk_widget_show(glwindow.glarea);

    gtk_main();
    return 0;
}
