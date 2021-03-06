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
#include <time.h>
#include <string.h>

static int slices = 16;
static int stacks = 16;
static int forma_display = 1;

int tam_vet_triangulos = 275084;
int tam_vet_vertices = 168534;
float vet_cores[] = {1, 0, 0};

float x_min, y_min, z_min, x_max, y_max, z_max;

int FPS = 0;
float tempo_inicial = 0.0, tempo_final = 0.0;

float vet_vertices[168534][3];
int vet_triangulos[275084][3];
float vet_normal[168534][3];

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
    int teste;

    if(arq == NULL){
        printf("Erro ao ler o arquivo.\n");
        return;
    }
    fscanf(arq, "%d %d", &n_vertices, &n_triangulos);

    float x, y, z;
    float nx, ny, nz;
    int index;


    while(i < n_vertices){

        fscanf(arq, "%d %f %f %f %f %f %f", &index, &x, &y, &z, &nx, &ny, &nz);
        vet_vertices[i][0] = x;
        vet_vertices[i][1] = y;
        vet_vertices[i][2] = z;

        vet_normal[i][0] = nx;
        vet_normal[i][1] = ny;
        vet_normal[i][2] = nz;

        i++;
    }

    i = 0;
    int tx, ty, tz;
    while(i < n_triangulos){
        fscanf(arq, "%d %d %d %d", &index, &tx, &ty, &tz);
        vet_triangulos[i][0] = tx;
        vet_triangulos[i][1] = ty;
        vet_triangulos[i][2] = tz;

        i++;
    }

    printf("END!\n");

}


void getMinMax(){
    x_min = vet_vertices[vet_triangulos[0][0]][0];
    y_min = vet_vertices[vet_triangulos[0][0]][1];
    z_min = vet_vertices[vet_triangulos[0][0]][2];

    x_max = vet_vertices[vet_triangulos[0][0]][0];
    y_max = vet_vertices[vet_triangulos[0][0]][1];
    z_max = vet_vertices[vet_triangulos[0][0]][2];

    for(int i = 0; i < tam_vet_triangulos; i++){
        for(int j = 0; j < 3; j++){
            if(x_min > vet_vertices[vet_triangulos[i][j]][0]) x_min = vet_vertices[vet_triangulos[i][j]][0];
            if(y_min > vet_vertices[vet_triangulos[i][j]][0]) y_min = vet_vertices[vet_triangulos[i][j]][1];
            if(z_min > vet_vertices[vet_triangulos[i][j]][0]) z_min = vet_vertices[vet_triangulos[i][j]][2];

            if(x_max < vet_vertices[vet_triangulos[i][j]][0]) x_max = vet_vertices[vet_triangulos[i][j]][0];
            if(y_max < vet_vertices[vet_triangulos[i][j]][0]) y_max = vet_vertices[vet_triangulos[i][j]][1];
            if(z_max < vet_vertices[vet_triangulos[i][j]][0]) z_max = vet_vertices[vet_triangulos[i][j]][2];
        }
    }
}

void printFile(){
    for(int i = 0; i < tam_vet_vertices; i++){
        //for(int j = 0; j < 3; j++){
        printf("%d %f %f %f %f %f %f\n", i, vet_vertices[i][0], vet_vertices[i][1], vet_vertices[i][2], vet_normal[i][0], vet_normal[i][1], vet_normal[i][2]);
        //}
    }


    for(int i = 0; i < tam_vet_triangulos; i++){
        //for(int j = 0; j < 3; j++){
        printf("%d %d %d %d\n", i, vet_triangulos[i][0], vet_triangulos[i][1], vet_triangulos[i][2]);
        //}
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
//    glFrustum(-ar, ar, -1.0, 1.0, 0.0001, 100.0);
    gluPerspective(60, ar, 0.01, 10000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

// considerar a maior dimensão

float caixa(){
    float d = (z_max - z_min) / 2;
    float retorno = ((y_max - y_min) / 2) / d;
    printf("Caixa: %f", retorno);
    return retorno;
}

static void display(void)
{
    const double elapsed_time = glutGet(GLUT_ELAPSED_TIME);
    const double a = (elapsed_time/1000)*30.0;
    int teste = 0;

    //float d_euclidiana = sqrt();

    tempo_inicial = elapsed_time - tempo_final;
    tempo_final = elapsed_time;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    glLoadIdentity(); // Limpa as tranformações anteriores
    glTranslatef(0, 0, -500);

    glPushMatrix();
    glRotated(a,0,0.5,0); // Rotaciona de 1 em 1 grau em torno do eixo Y

    glTranslatef(-(x_min + x_max) / 2,
                    -(y_min + y_max) / 2,
                    -(z_min + z_max) / 2);

    for(int i = 0; i < tam_vet_triangulos; i++){
        glBegin(GL_TRIANGLES);
        for(int j = 0; j < 3; j++){
            glNormal3f(vet_normal[vet_triangulos[i][j]][0],
                    vet_normal[vet_triangulos[i][j]][1],
                    vet_normal[vet_triangulos[i][j]][2] );

            glVertex3f( vet_vertices[vet_triangulos[i][j]][0],
                        vet_vertices[vet_triangulos[i][j]][1],
                        vet_vertices[vet_triangulos[i][j]][2]);
        }
        glEnd();
    }
    glPopMatrix();



    FPS++;
    //tempo_final = time(NULL);
    char buffer[20];
//    printf("Tempo Inicial: %f  |  Tempo Final: %f\n", tempo_inicial, tempo_final);
    if(tempo_inicial > 1.0){
//        printf("Tempo Inicial: %f", tempo_inicial);
//        scanf("%d", &teste);
        itoa((tempo_inicial*10), buffer, 10);
//        FPS = 0;
//        tempo_inicial = tempo_final;
        printGL(20, 20, buffer);
        printGL(10, 450, "MODO: GLBEGIN");
    }
    glutSwapBuffers();
}

static void draw(void)
{
    const double elapsed_time = glutGet(GLUT_ELAPSED_TIME);
    const double a = (elapsed_time/1000)*30.0;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3d(1,0,0);
    glLoadIdentity(); // Limpa as tranformações anteriores
    glTranslatef(0, 0, -500);

    glEnableClientState(GL_NORMAL_ARRAY);
//    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(GL_FLOAT, 0, vet_normal);
//    glColorPointer(3, GL_FLOAT, 0, vet_cores);
    glVertexPointer(3, GL_FLOAT, 0, vet_vertices);

    glPushMatrix();
    glRotated(a,0,0.5,0); // Rotaciona de 1 em 1 grau em torno do eixo Y

    glTranslatef(-(x_min + x_max) / 2,
                    -(y_min + y_max) / 2,
                    -(z_min + z_max) / 2);

    glDrawElements(GL_TRIANGLES, tam_vet_triangulos * 3, GL_UNSIGNED_INT, vet_triangulos);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
//    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    printGL(10, 450, "MODO: DRAW ELEMENTS");
    glutSwapBuffers();
}

static void generic_display(void){
    if(forma_display == 1) display();
    if(forma_display == 2) draw();
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
        case 'r':
            if(forma_display == 1){
                forma_display = 2;
            }else{
                forma_display = 1;
            }
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
//    lerArquivo("C:\\Users\\20142bsi0054\\Documents\\TECG\\bunny.msh.txt");
    lerArquivo("C:\\Users\\20142bsi0054\\Documents\\TECG\\DeLorean.msh");
    //printFile();
//     lerArquivo("C:\\Users\\Cristian\\Documents\\IFES\\bunny.msh.txt");
    getMinMax(); // obtém as coordenadas (x,y,z) min e max
//    caixa();

//    printf("X_MIN: %f, Y_MIN: %f, Z_MIN: %f\n", x_min, y_min, z_min);
//    printf("Média: %f\n", (x_min + x_max) / 2);
//    printf("X_MAX: %f, Y_MAX: %f, Z_MAX: %f\n", x_max, y_max, z_max);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);         // Size of display area, in pixels.
    glutInitWindowPosition(10, 10);     // Location of window in screen coordinates.
    glutCreateWindow("BUNNY");           // Parameter is window title.


    glutDisplayFunc(generic_display);
    glutReshapeFunc(resize);
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
