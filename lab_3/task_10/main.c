#include "functions.h"

int main()
{
    Node *root = (Node *)malloc(sizeof(Node));
    init_tree(root);
    build_tree(root, "A(B(E(G,T,R(W,Z)),F(L,M)),C)");
    print_tree(root, 0);

    return 0;
}