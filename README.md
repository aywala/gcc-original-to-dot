# gcc-original-to-dot

## English

transform gcc .original file to dot file so that we can use graphviz to visualize ast;
you can use gcc `-fdump-tree-all-raw` to get the .original file.

**compile:**

``g++ -O3 gen-ast-dot -o genASTDot``

**use:**

``./genASTDot example-example.c.005t.original``

this command will generate **ast.dot** file

you can use `dot -Tpng ast.dot -o ast.png` to generate an ast image;（graphviz is needed）

---
## 中文

将 gcc `-fdump-tree-all-raw` 选项生成的 .orignal 文件转换成能被graphviz用来生成图片的dot文件。

**编译:**

``g++ -O3 gen-ast-dot -o genASTDot``

**使用:**

``./genASTDot example-example.c.005t.original``

这个命令会生成 **ast.dot** 文件

然后使用 `dot -Tpng ast.dot -o ast.png` 命令生成AST图;（需要安装graphviz）