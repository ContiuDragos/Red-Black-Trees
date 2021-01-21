#include <iostream>
#include <cstring>
#include "Profiler.h"
#include <time.h>

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5
Profiler p("DynamicOrderStatistics");
typedef struct _Node
{
    int key;
    char color[10];
    struct _Node *left, *right, *parent;
    int size;
}Node;
typedef struct {
    Node *root;
    Node *nil;
}redBlackTree;
int n;
redBlackTree* tree=(redBlackTree*)calloc(1,sizeof(redBlackTree));
Node* alocaNode(int value, char* color)
{
    Node *p=(Node*)calloc(1,sizeof(Node));
    if(p == nullptr)
    {
        printf("Not enough memory\n");
        exit(-1);
    }
    p->key=value;
    p->size=1;
    strcpy(p->color,color);
    return p;
}
void PrettyPrint(Node *root,int nivel)
{
    if(root!=nullptr && root->key!=-1)
    {
        for(int i=0;i<nivel;i++)
            printf("   ");
        printf("%d %s size:%d\n",root->key,root->color,root->size);
        PrettyPrint(root->left,nivel+1);
        PrettyPrint(root->right,nivel+1);
    }
}
void leftRotate( Node *x, int *count)
{
    (*count)+=2;
    Node *y=(*x).right;
    if(y!=tree->nil)
    {
        (*x).right=(*y).left;
        if((*y).left!=tree->nil)
            (*y).left->parent=x;
        (*y).parent=(*x).parent;
        (*count)+=4;
        if(x->parent==tree->nil)
            tree->root=y;
        else
        {
            (*count)+=2;
            if(x==(*x).parent->left)
                (*x).parent->left=y;
            else
                (*x).parent->right=y;
        }
        (*y).left=x;
        (*x).parent=y;
        (*y).size=(*x).size;
        (*x).size= (*x).left->size + (*x).right->size + 1;
        (*count)+=4;
    }
    else
    {
        (*count)++;
        if(x->parent->parent==tree->nil) {
            tree->root = x;
            (*count)++;
        }
        (*x).parent->parent->left=x;
        (*x).right=x->parent;
        (*x).parent=x->parent->parent;
        (*x).left->parent=x;
        (*x).right->left=tree->nil;
        y=x->right;
        (*y).size=(*x).size;
        (*x).size= (*x).left->size + (*x).right->size + 1;
        (*count)+=8;
    }

}
void rightRotate( Node *x, int *count)
{
    Node *y=(*x).left;
    (*count)+=2;
    if(y!=tree->nil)
    {
        (*x).left=(*y).right;
        if((*y).right!=tree->nil)
            (*y).right->parent=x;
        (*y).parent=(*x).parent;
        (*count)+=4;
        if((*x).parent==tree->nil)
            tree->root=y;
        else{
            (*count)+=2;
            if(x==(*x).parent->right)
                (*x).parent->right=y;
            else
                (*x).parent->left=y;
        }
        (*y).right=x;
        (*x).parent=y;
        (*y).size=(*x).size;
        (*x).size= (*x).left->size + (*x).right->size + 1;
        (*count)+=4;
    }
    else
    {
        (*count)++;
        if(x->parent->parent==tree->nil)
        {
            tree->root=x;
            (*count)++;
        }
        x->parent->parent->right=x;
        (*x).parent->right=tree->nil;
        x->left=x->parent;
        x->parent=x->parent->parent;
        x->left->parent=x;
        y=x->left;
        (*y).size=y->left->size+y->right->size+1;
        (*x).size= (*x).left->size + (*x).right->size + 1;
        (*count)+=8;
    }

}
void redBlackInsertFix( Node *z)
{
    int nimic;
    while(strcmp(z->parent->color,"rosu")==0)
    {
        if(z->parent==z->parent->parent->left)
        {
            Node *y=z->parent->parent->right;
            if(strcmp(y->color,"rosu")==0)
            {
                ///case 1
                strcpy(z->parent->color,"negru");
                strcpy(y->color,"negru");
                strcpy(z->parent->parent->color,"rosu");
                z=z->parent->parent;
            }
            else
            {
                ///case 2
                if(z==z->parent->right)
                {
                    z=z->parent;
                    leftRotate(z,&nimic);
                }
                ///case 3
                strcpy(z->parent->color,"negru");
                strcpy(z->parent->parent->color,"rosu");
                rightRotate(z->parent->parent,&nimic);
            }
        }
        else
        {
            ///case 1
            Node *y=z->parent->parent->left;
            if(strcmp(y->color,"rosu")==0)
            {
                strcpy(z->parent->color,"negru");
                strcpy(y->color,"negru");
                strcpy(z->parent->parent->color,"rosu");
                z=z->parent->parent;
            }
            else
            {
                ///case 2
                if(z==z->parent->left)
                {
                    z=z->parent;
                    rightRotate(z,&nimic);
                }
                ///case 3
                strcpy(z->parent->color,"negru");
                strcpy(z->parent->parent->color,"rosu");
                leftRotate(z->parent->parent,&nimic);
            }
        }
    }
    strcpy(tree->root->color,"negru");
}
void redBlackInsert( int value)
{
    Node *y=tree->nil;
    Node *x=tree->root;
    Node *z=alocaNode(value,"rosu");

    ///cauta locului noului nod in arbore
    ///il insereaza ca si frunza
    while(x!=tree->nil)
    {
        y=x;
        y->size++;
        if(z->key<x->key)
            x=x->left;
        else
            x=x->right;
    }
    z->parent=y;
    if(y==tree->nil)
        tree->root=z;
    else
    {
        if(z->key<y->key)
            y->left=z;
        else
            y->right=z;
    }
    z->left=tree->nil;
    z->right=tree->nil;
    redBlackInsertFix(z);

}
void determineSize( Node* root)
{
    if(root->left!=tree->nil)
        determineSize( root->left);
    if(root->right!=tree->nil)
        determineSize( root->right);
    root->size=root->left->size+root->right->size+1;
}
void redBlackTransplant( Node *x, Node *y, int *count)
{
    (*count)++;
    if(x->parent==tree->nil)
        tree->root=y;
    else
    {
        if(x==x->parent->left)
            x->parent->left=y;
        else
            x->parent->right=y;
    }
    (*count)+=3;
    if(y!=tree->nil)
    {
        y->parent=x->parent;
        (*count)++;
    }

}
Node* treeMinim(Node *root, int *count)
{
    while(root->left!=tree->nil)
    {
        root=root->left;
        (*count)+=2;
    }
    (*count)++;
    return root;

}
void redBlackDeleteFix( Node *x, int *count)
{
    Node *w=NULL;
    while(x!=tree->root && strcmp(x->color,"black")==0)
    {
        (*count)+=2;
        if(x==x->parent->left)
        {
            (*count)++;
            w=x->parent->right;
            (*count)++;
            if(strcmp(w->color,"rosu")==0)
            {
                strcpy(w->color,"negru");
                strcpy(x->parent->color,"rosu");
                leftRotate(x->parent,count);
                w=x->parent;
                (*count)+=3;
            }
            (*count)+=2;
            if(strcmp(w->left->color,"negru")==0 && strcmp(w->right->color,"negru")==0)
            {
                strcpy(w->color,"rosu");
                x=x->parent;
                (*count)+=2;
            }
            else
            {
                (*count)++;
                if(strcmp(w->right->color,"negru")==0)
                {
                    strcpy(w->left->color,"negru");
                    strcpy(w->color,"rosu");
                    rightRotate(w,count);
                    w=x->parent->right;
                    (*count)+=3;

                }
                strcpy(w->color,x->parent->color);
                strcpy(x->parent->color,"negru");
                strcpy(w->right->color,"negru");
                leftRotate(x->parent,count);
                (*count)+=3;

                x=tree->root;
                (*count)++;
            }
        }
        else
        {
            (*count)++;
            if(x==x->parent->right)
            {
                w=x->parent->left;
                (*count)++;
                if(strcmp(w->color,"rosu")==0)
                {
                    strcpy(w->color,"negru");
                    strcpy(x->parent->color,"rosu");
                    leftRotate(x->parent,count);
                    w=x->parent;
                    (*count)+=3;
                }
                (*count)+=2;
                if(strcmp(w->right->color,"negru")==0 && strcmp(w->left->color,"negru")==0)
                {
                    strcpy(w->color,"rosu");
                    x=x->parent;
                    (*count)+=2;
                }
                else
                {
                    (*count)++;
                    if(strcmp(w->left->color,"negru")==0)
                    {
                        strcpy(w->right->color,"negru");
                        strcpy(w->color,"rosu");
                        rightRotate(w,count);
                        w=x->parent->left;
                        (*count)+=3;
                    }
                    strcpy(w->color,x->parent->color);
                    strcpy(x->parent->color,"negru");
                    strcpy(w->left->color,"negru");
                    leftRotate(x->parent,count);
                    x=tree->root;
                    (*count)+=4;
                }
            }
        }
    }
    strcpy(x->color,"negru");
    (*count)++;
}
void redBlackDelete( Node *z)
{
    Operation oprDelete=p.createOperation("Operatii-Delete",n);
    int count=0;
    Node *y=z;
    Node *x=NULL;
    Node *aux=z;
    char originalColor[20];
    strcpy(originalColor,y->color);
    ///modific size-urile nodurilor pana la radacina
    while(aux->parent!=tree->nil)
    {
        oprDelete.count(3);
        aux=aux->parent;
        aux->size--;
    }
    oprDelete.count();
    ///daca fiul stang e nil
    if(z->left==tree->nil)
    {
        count=0;
        x=z->right;
        redBlackTransplant(z,z->right,&count);  ///elimina nodul z si il inlocuieste cu z->right
        oprDelete.count(count);
    }
    else
    {
        oprDelete.count();
        ///daca fiul drept e nil
        if(z->right==tree->nil)
        {
            count=0;
            x=z->left;
            redBlackTransplant(z,z->left,&count);           ///elimina nodul z si il inlocuieste cu z->left
            oprDelete.count();
        }
        else
        {
            ///daca are si fiu stanga si dreapta
            count=0;
            y=treeMinim(z->right,&count);           ///cauta minimul
            oprDelete.count(count);
            aux=y;
            ///asigur size-ul
            while(aux->parent!=z)
            {
                oprDelete.count(3);
                aux=aux->parent;
                aux->size--;
            }
            strcpy(originalColor,y->color);
            x=y->right;
            ///daca minimul e fiul nodului pe care il stergem
            if(y->parent==z)
            {
                x->parent=y;
                oprDelete.count();
            }
            else
            {
                count=0;
                redBlackTransplant(y,y->right,&count);
                y->right=z->right;
                y->right->parent=y;
                oprDelete.count(count+2);

            }
            count=0;
            redBlackTransplant(z,y,&count);
            y->left=z->left;
            y->left->parent=y;
            y->size=1+y->left->size+y->right->size;
            strcpy(y->color,z->color);
            oprDelete.count(4);
        }
    }
    count=0;
    if(strcmp(originalColor,"negru")==0)
        redBlackDeleteFix(x,&count);
    oprDelete.count(count);
}
Node* osSelect(Node* x, int i)
{
    Operation oprSelect=p.createOperation("Operatii-Select",n);
    int r =x->left->size+1;
    oprSelect.count();
    if(r==i)
        return x;
    else
    {
        oprSelect.count();
        if(i<r)
            return osSelect(x->left,i);
        return osSelect(x->right,i-r);
    }
}
void shuffle(int **array)
{
    if (n > 1)
    {
        int i;
        for (i = 0; i < n - 1; i++)
        {
            int j = rand()%(n-1)+1;
            int t = (*array)[j];
            (*array)[j] = (*array)[i];
            (*array)[i] = t;
        }
    }
}
void demo()
{
    ///int v[]={26,17,41,14,21,30,47,10,16,19,21,28,38,7,12,15,20,35,39,3};
    int v[]={11,2,14,1,7,15,5,8,4};
    ///int v[]={1,2,3,4,5,6,7,8,9};
    n=sizeof(v)/sizeof(n);

    tree->nil=alocaNode(-1,"negru");          ///NIL va fi -1
    tree->nil->size=0;
    tree->root=tree->nil;
    tree->nil->right=tree->nil;
    tree->nil->left=tree->nil;

    for(int i=0;i<n;i++)
        redBlackInsert(v[i]);

    PrettyPrint(tree->root,0);


    Node *x = osSelect(tree->root,5);
    printf("The node with %d-th smallest key is %d\n",5,x->key);
    redBlackDelete(x);
    PrettyPrint(tree->root,0);

    x = osSelect(tree->root,3);
    printf("The node with %d-th smallest key is %d\n",3,x->key);
    redBlackDelete(x);
    PrettyPrint(tree->root,0);

    x = osSelect(tree->root,1);
    printf("The node with %d-th smallest key is %d\n",1,x->key);
    redBlackDelete(x);
    PrettyPrint(tree->root,0);


}
void perf()
{
    int count1=0,count2=0;
    srand(time(0));
    int *v=(int*)calloc(MAX_SIZE,sizeof(int));
    if(tree==nullptr || v==nullptr)
    {
        printf("Not enough memory");
        exit(-1);
    }
    tree->nil=alocaNode(-1,"negru");          ///NIL va fi -1
    tree->nil->size=0;
    tree->root=tree->nil;
    tree->nil->right=tree->nil;
    tree->nil->left=tree->nil;

    for(int i=0;i<MAX_SIZE;i++)
        v[i]=i+1;
    int nrNodes;
    Node *x=NULL;
    for( n=100;n<=MAX_SIZE;n+=STEP_SIZE)
    {
        nrNodes=n-1;
        printf("n=%d\n",n);
        for(int i=0;i<NR_TESTS;i++)
        {
            shuffle(&v);
            for(int j=0;j<n;j++)
                redBlackInsert(v[j]);
            for(int j=0;j<n && nrNodes>0;j++)
            {
                x=osSelect(tree->root,rand()%nrNodes+1);
                redBlackDelete(x);
                nrNodes--;
            }
        }
    }

    p.divideValues("Operatii-Select", NR_TESTS);
    p.divideValues("Operatii-Delete", NR_TESTS);
}
int main() {
    demo();
    ///perf();
    ///p.showReport();
    return 0;
}
