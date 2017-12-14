//HASAN ONAT TOPRAK-2015555063
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define getint(x) scanf("%d",&x)
enum SearchType{PREORDER=1,INORDER,POSTORDER};


typedef struct binarytree{
  int num;
  struct binarytree* left;
  struct binarytree* right;
}btree;

void addToTree (btree** start, int inputnumber){
  if(*start == NULL){*start = (btree*)malloc(sizeof(btree));(*start)->num = inputnumber;(*start)->left = NULL;(*start)->right=NULL;}
  else if(inputnumber < (*start)->num){addToTree(&(*start)->left,inputnumber);}
  else{addToTree(&(*start)->right,inputnumber);}
}


void menu_addToTree(btree** root){
  int selectedValue;
  printf("Enter Input Value = ");
  getint(selectedValue);
  addToTree(root,selectedValue);
}


void showNode(btree* node) {
  printf("  --  Value = %d",node->num);
  if(node->left){printf("  Left Value = %d",node->left->num);}
  if(node->right){printf("  Right Value = %d",node->right->num);}
  printf("\n");
}


void showTree(btree* start,int type){
  static int emptyCheck = 1;
  static int degree = 0;

  if(start != NULL){
    emptyCheck = 0;
    if(type == PREORDER){degree ? printf("%dth DEGREE",degree):printf("ROOT DEGREE");showNode(start);}
    degree++;
    showTree(start->left,type);
    degree--;
    if(type == INORDER){degree ? printf("%dth DEGREE",degree):printf("ROOT DEGREE");showNode(start);}
    degree++;
    showTree(start->right,type);
    degree--;
    if(type == POSTORDER){degree ? printf("%dth DEGREE",degree):printf("ROOT DEGREE");showNode(start);}
  }
  else if (emptyCheck){printf("Tree is Empty\n");}
}


void menu_showTree(btree* root){
  int selectedSearchType;
  printf("1-Preorder Traversal\n2-Inorder Traversal\n3-Postorder Traversal\n");
  getint(selectedSearchType);
  showTree(root,selectedSearchType);
}

/*int getTreeDegree(btree* start){
  static int emptyCheck = 1;
  static int degree = 0;
  static int maxDegree = 0;

  if(start != NULL){
    emptyCheck = 0;

    if(maxDegree < degree){maxDegree = degree;}
    degree++;
    getTreeDegree(start->left);
    getTreeDegree(start->right);
    degree--;
  }
  else if (emptyCheck){printf("Tree is Empty\n");}
  return maxDegree;
}
int powerSumTwo(int inp){
  int out = 0;
  for(int i = inp;i > 0;i--){
    out += (int)pow(2,i);
  }
  return out;
}
void showTreeGraphical(btree* root) {
  int iterator = 14;
  for(int i = 0;i<iterator;i++){printf(" ");}printf("%d\n",root->num);
  for(int i = 0;i<8;i++){printf(" ");}for(int i = 0;i<8;i++){printf("_");}printf("|\n");
}*/


int main(int argc, char const *argv[]) {
  btree* rootNode = NULL;
  int selectedMode;
  while(1){
      printf("1-Insertion\n2-Show the Tree\n3-Exit\n");
      getint(selectedMode);
      switch (selectedMode) {
        case 1:menu_addToTree(&rootNode);break;
        case 2:menu_showTree(rootNode);break;
        case 3:printf("Exiting ...\n");exit(0);
        //case 4:showTreeGraphical(rootNode);break;
        default : printf("\n\nInvalid Input!!!\n\n");continue;
      }
      while(getchar() != '\n');
  }

  printf("\n\nProgram End\n\n");
  return 0;

}
