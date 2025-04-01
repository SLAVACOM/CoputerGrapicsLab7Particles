#include <GL/glut.h>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Particle {
    float x, y, z;
    float velocityX, velocityY, velocityZ;
    float life;

    Particle(float startX, float startY, float startZ) {
        reset(startX, startY, startZ);
    }

    void update() {
        y += velocityY;
        x += velocityX;
        z += velocityZ;
        velocityY -= 0.001f; // Умеренная гравитация
        life -= 0.008f;
        if (life <= 0 || y < -0.5f) reset(0.0f, -0.5f, 0.0f);
    }

    void reset(float startX, float startY, float startZ) {
        x = startX + (rand() % 40 - 20) / 400.0f; // Больше случайности в X
        y = startY;
        z = startZ + (rand() % 40 - 20) / 400.0f; // Больше случайности в Z
        velocityX = (rand() % 20 - 10) / 1000.0f; // Расходящиеся частицы
        velocityY = (rand() % 30) / 1000.0f + 0.015f; // Меньшая высота
        velocityZ = (rand() % 20 - 10) / 1000.0f; // Добавлено разлетание по сторонам
        life = 1.0f;
    }
};

std::vector<Particle> particles;

void initParticles(int numParticles) {
    for (int i = 0; i < numParticles; i++) {
        particles.push_back(Particle(0.0f, -0.5f, 0.0f));
    }
}

void updateParticles() {
    for (auto& p : particles) {
        p.update();
    }
}

void drawParticles() {
    glPointSize(2.0f); // Уменьшенные частицы
    glBegin(GL_POINTS);
    for (auto& p : particles) {
        glColor4f(0.6f, 0.8f, 1.0f, p.life);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -2.0f);

    drawParticles();
    updateParticles();
    glBegin(GL_QUADS); 
    glColor3f(1.0f, 1.0f, 1.0f); 


    glVertex2f(-0.05f, -1.5f); 
    glVertex2f(0.05f, -1.5f); 


    glVertex2f(0.05f, -0.51f); // Правая верхняя точка
    glVertex2f(-0.05f, -0.51f); // Левая верхняя точка

    glEnd();  
    glutSwapBuffers();
}

void init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Particle Fountain");

    init();

    initParticles(200);

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
