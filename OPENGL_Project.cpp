#include <GL/glut.h>
#include <math.h>

/*
student1: Abdallah Mahmoud Hreba --> 20211062
student2: Youssef Mohamed abdelazim --> 20221257
student3: Nour eldeen khaled sharkawy --> 20210432
*/

// Global variables
float positions[4][2] = { {0.7f, 0.4f}, {0.7f, -0.4f}, {-0.7f, 0.4f}, {-0.7f, -0.4f} }; // For the windows
float doorAngle = 0.0f;
float windowAngle = 0.0f;
float bicycleAngle = 0.0f; // Used for motion around the building
float rightWheelRotation = 0.0f;
float leftWheelRotation = 0.0f;
bool animate = false;
int lastMouseX, lastMouseY;
float cameraAngleX = 45.0f;
float cameraAngleY = 45.0f;
float cameraDistance = 8.0f;

void initGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.5, 0.0, 1.0);
}

void displayBuilding() {
    glColor3f(0.75, 0.75, 0.75);
    glPushMatrix();
    glScalef(2.0f, 2.0f, 2.0f); // Larger building size
    glutSolidCube(1.0f);
    glPopMatrix();

    // drawing The roof part of the building
    glColor3f(0.5f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glScalef(2.1f, 0.2f, 2.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Drawing a line to seperate the floors
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(-1.0f, 0.0f, 1.01f);
    glVertex3f(1.0f, 0.0f, 1.01f);
    glEnd();
}

// function to make our centered door in the building
void displayDoor() {
    glColor3f(0.4f, 0.2f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 1.01f);
    glRotatef(doorAngle, 0.0f, 1.0f, 0.0f);
    glScalef(0.3f, 0.7f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// function to make the 4 windows in our building
void displayWindows() {
    for (int i = 0; i < 4; i++) {
        glColor3f(0.3f, 0.4f, 0.6f);
        glPushMatrix();
        glTranslatef(positions[i][0], positions[i][1], 1.01f);
        glRotatef(windowAngle, 0.0f, 1.0f, 0.0f);
        glScalef(0.2f, 0.2f, 0.05f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
}

//function to draw the circular road around our building
void displayRoad() {
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    glBegin(GL_QUADS);
    for (int i = 0; i < 360; i += 10) {
        float angle1 = i * M_PI / 180.0f;
        float angle2 = (i + 10) * M_PI / 180.0f;
        glVertex3f(2.0 * cos(angle1), 0.0f, 2.0 * sin(angle1));
        glVertex3f(2.2 * cos(angle1), 0.0f, 2.2 * sin(angle1));
        glVertex3f(2.2 * cos(angle2), 0.0f, 2.2 * sin(angle2));
        glVertex3f(2.0 * cos(angle2), 0.0f, 2.0 * sin(angle2));
    }
    glEnd();
    glPopMatrix();
}

//function to make our bicycle 
void displayBicycle() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    // Putting the bicycle on the road using the appropriate angle
    glTranslatef(2.0 * cos(bicycleAngle), -0.85f, 2.0 * sin(bicycleAngle));

    // Calculating the angle to the next position
    float nextX = 2.0 * cos(bicycleAngle + 0.05f);
    float nextZ = 2.0 * sin(bicycleAngle + 0.05f);
    float angle = atan2(nextZ - (2.0 * sin(bicycleAngle)), nextX - (2.0 * cos(bicycleAngle))) * 180.0 / M_PI;

    // Rotating the bicycle
    glRotatef(angle, 0.0f, -1.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-0.2f, 0.0f, 0.0f);
    glRotatef(leftWheelRotation, 0.0f, 1.0f, 0.0f);
    glutWireTorus(0.05, 0.1, 12, 24);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.2f, 0.0f, 0.0f);
    glRotatef(rightWheelRotation, 0.0f, 1.0f, 0.0f);
    glutWireTorus(0.05, 0.1, 12, 24);
    glPopMatrix();

    // Drawing frames
    glBegin(GL_LINES);
    glVertex2f(-0.2f, 0.0f);
    glVertex2f(0.2f, 0.0f);
    glVertex2f(-0.2f, 0.0f);
    glVertex2f(0.0f, 0.1f);
    glVertex2f(0.2f, 0.0f);
    glVertex2f(0.0f, 0.1f);
    glEnd();
    glPopMatrix();
}

//function responsible for displaying our scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Handling the Camera position and orientation for our 3D realistic scene
    gluLookAt(
        cameraDistance * cos(cameraAngleX * 3.14 / 180.0) * sin(cameraAngleY * 3.14 / 180.0),
        cameraDistance * cos(cameraAngleY * 3.14 / 180.0),
        cameraDistance * sin(cameraAngleX * 3.14 / 180.0) * sin(cameraAngleY * 3.14 / 180.0),
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );

    displayRoad();
    displayBuilding();
    displayDoor();
    displayWindows();
    displayBicycle();
    glutSwapBuffers();
}

// Function to help us reshape our scene
void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Handling the keyboard input
void keyboard(unsigned char key, int x, int y) {
    const float doorMaxAngle = 90.0f;
    const float windowMaxAngle = 60.0f;
    switch (key) {
        case 'o':
            if (doorAngle < doorMaxAngle) {
                doorAngle += 10.0f;
            }
            break;
        case 'c':
            if (doorAngle > 0) doorAngle -= 10.0f;
            break;
        case 'O':
            if (windowAngle < windowMaxAngle) {
                windowAngle += 10.0f;
            }
            break;
        case 'C':
            if (windowAngle > 0) windowAngle -= 10.0f;
            break;
        case 'r':
            rightWheelRotation += 10.0f;
            break;
        case 'l':
            leftWheelRotation += 10.0f;
            break;
        case 'f': 
            bicycleAngle += 0.1f;
            if (bicycleAngle < 0) bicycleAngle -= 2 * M_PI;
            break;
        case 'b': 
            bicycleAngle -= 0.1f;
            if (bicycleAngle > 2 * M_PI) bicycleAngle += 2 * M_PI;
            break;
    }
    glutPostRedisplay();
}

//function to deal with our mouse input
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        animate = true;
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        animate = false;
    }
    lastMouseX = x;
    lastMouseY = y;
}

void motion(int x, int y) {
    int deltaX = x - lastMouseX;
    int deltaY = y - lastMouseY;
    cameraAngleX += deltaX * 0.1;
    cameraAngleY += deltaY * 0.1;
    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

void timer(int value) {
    if (animate) {
        bicycleAngle += 0.05f;
        if (bicycleAngle > 2 * M_PI) bicycleAngle -= 2 * M_PI;
    }
    glutPostRedisplay();
    glutTimerFunc(100, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("OpenGL Project");
    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
