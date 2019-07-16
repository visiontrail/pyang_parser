## 1 重点关注 ##

### 1.1 当前版本限制内容
**Fork后修改说明——根据YANG文件自动生成C语言结构的限制内容：**

  - 每个submodule会生成对应的头文件和cpp文件，需要注意以下几点：
    - 主grouping最多可以包含两级container或list嵌套
    - 主grouping的第一级不能是list
    - 主grouping中list嵌套list的时候，那个子list只能包含叶子节点，如果需要复杂的组合，可单独再定义一个grouping
    - 除主grouping外，一个grouping中，只支持一级container或list，如果多于两级，则这种场景再单独编写一个grouping
    - 一个container或者list中只要包含了uses，就不要再包含同级的container，这种场景最好再单独写一个grouping（可以包含同级leaf）

  - 暂不支持vaildate函数(Check函数)

### 1.2 运行与使用说明
- 使用python2.7运行
- Fork后新增模块：
  - **xtypedef** : 解析submodule下所有的grouping，并将其转化为C语言的struct结构体，与submodule名称相同的grouping会生成为一个类型
  - **pathheader** : 生成一个module下边所有yang节点的树型结构全路径
  - **callbackreg** : 与pathheader配合使用，如果是container或者list，会自动生成对应的回调函数
  - **callbackregheader** : 与pathheader配合使用，如果是container或者list，会自动生成对应回调函数的函数声明

---


## 2 Yang模型编写规范 ##
优先参见 **1.1** 中的限制说明

### 2.1 xtypedef模块的编写规范


## 3 原pyang支持的特性说明
