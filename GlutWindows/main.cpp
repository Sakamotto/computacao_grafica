/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static int slices = 16;
static int stacks = 16;

int tam_vet_triangulos = 69664;
int tam_vet_vertices = 34834;

int FPS = 0, tempo_inicial = time(NULL), tempo_final;

float vet_vertices[34834][6];
int vet_triangulos[69664][3];

//Função que desenha em tela nas posições X e Y, um texto.
static void printGL(int x, int y, char *str)
{

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 640, 0, 480, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glRasterPos2i(x,y);
    glColor3f(1,1,1);
    int len = (int) strlen(str);

    for (int i=0; i<len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
    }
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void lerArquivo(char* path){
    FILE *arq = fopen(path, "rt");
    int i = 0;
    char linha[100];
    char* result;
    int n_vertices, n_triangulos;

    if(arq == NULL){
        printf("Erro ao ler o arquivo.\n");
        return;
    }

    result = fgets(linha, 100, arq);
    n_vertices = atoi(strtok(result, " "));
    n_triangulos = atoi(strtok(NULL, " "));

    while(i < n_vertices){
        char* aux_token;
        int j = 0;
        result = fgets(linha, 100, arq);
        aux_token = strtok(result, " ");
        aux_token = strtok(NULL, " ");

        while(aux_token != NULL){
            vet_vertices[i][j] = atof(aux_token);
            aux_token = strtok(NULL, " ");
            j++;
        }
        i++;
    }

    i = 0;
    while(!feof(arq)){
        char* aux_token;
        int j = 0;
        result = fgets(linha, 100, arq);
        aux_token = strtok(result, " ");
        aux_token = strtok(NULL, " ");

        while(aux_token != NULL){
            vet_triangulos[i][j] = atoi(aux_token);
            aux_token = strtok(NULL, " ");
            j++;
        }
        i++;
    }

}

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    printf("Resize: %d, %d ", width, height);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    int teste = 0;
    for(int i = 0; i < tam_vet_triangulos; i++){
        glBegin(GL_TRIANGLES);
        for(int j = 0; j < 3; j++){
            glVertex3f( vet_vertices[vet_triangulos[i][j]][0],
                    vet_vertices[vet_triangulos[i][j]][1],
                    vet_vertices[vet_triangulos[i][j]][2] );

            glNormal3f(vet_vertices[vet_triangulos[i][j]][3],
                    vet_vertices[vet_triangulos[i][j]][4],
                    vet_vertices[vet_triangulos[i][j]][5] );
        }
        glEnd();
    }
    glRotated(0.2,0,0.5,0); // Rotaciona de 1 em 1 grau em torno do eixo Y

    FPS++;
    tempo_final = time(NULL);
    char buffer[20];
    if(tempo_final - tempo_inicial > 0){
        itoa((FPS / (tempo_final - tempo_inicial)), buffer, 10);
        FPS = 0;
        tempo_inicial = tempo_final;
    }

    printGL(20, 20, buffer);
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{
//    lerArquivo("C:\\Users\\Aevo\\Documents\\Cristian\\Trabalho_TECG\\bunny.msh.txt");
    lerArquivo("C:\\Users\\Cristian\\Documents\\IFES\\bunny.msh.txt");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);         // Size of display area, in pixels.
    glutInitWindowPosition(10, 10);     // Location of window in screen coordinates.
    glutCreateWindow("BUNNY");           // Parameter is window title.

    //glutDisplayFunc(display);

    glutDisplayFunc(display);
    //glutReshapeFunc(resize);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    glutMainLoop();

    return EXIT_SUCCESS;
}
