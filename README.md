# gcc-original-to-dot

## English

support gcc 12 and other recent versions of gcc. 

transform gcc .original file to dot file so that we can use graphviz to visualize ast;
you can use gcc `-fdump-tree-all-raw` to get the .original file.

**compile:**

``g++ -O3 gen-ast-dot -o genASTDot``

**use:**

``./genASTDot example-example.c.005t.original``

this command will generate some **.dot** file, each function generate one;

you can use `dot -Tpng ast_example.dot -o ast_example.png` to generate an ast image;（graphviz is needed）

---
## 中文

支持 gcc 12 以及其它最近版本的 gcc。

将 gcc `-fdump-tree-all-raw` 选项生成的 .orignal 文件转换成能被 graphviz 用来生成图片的 dot 文件。

**编译:**

``g++ -O3 gen-ast-dot -o genASTDot``

**使用:**

``./genASTDot example-example.c.005t.original``

这个命令会生成若干个 **.dot** 文件，每个函数生成一个。

然后使用 `dot -Tpng ast_example.dot -o ast_example.png` 命令生成 AST 图;（需要安装 graphviz）