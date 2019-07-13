## 重点关注 ##

**当前限制**

  - 当前版本生成的头文件中的read函数，最多支持两级。所以当前设计的yang，需要注意以下几点：
    - 一个container或者list中只要包含了uses，就不要再包含同级的container，这种场景最好再单独写一个grouping（可以包含同级leaf）
    - 主grouping最多可以包含两级container或list嵌套
    - 主grouping的第一级不能是list
    - 主groupinglist嵌套list的时候，那个子list只能包含叶子节点
    - 除主grouping外，一个grouping中，只支持一级container或list，如果多于两级，则这种场景再单独编写一个grouping

  - 暂不支持vaildate函数(Check函数)

---


## Yang模型编写规范 ##

### xtypedef模块：


## 安装 ##


## 支持的特性 ##

  * Validate YANG modules.
  * Convert YANG modules to YIN, and YIN to YANG.
  * Translate YANG data models to DSDL schemas, which can be used for
    validating various XML instance documents. See
    [InstanceValidation](https://github.com/mbj4668/pyang/wiki/InstanceValidation).
  * Translate YANG data models to XSD.
  * Generate UML diagrams from YANG models. See
    [UMLOutput](https://github.com/mbj4668/pyang/wiki/UMLOutput) for
    an example.
  * Generate compact tree representation of YANG models for quick
    visualization. See
    [TreeOutput](https://github.com/mbj4668/pyang/wiki/TreeOutput) for
    an example.
  * Generate a skeleton XML instance document from the data model.
  * Schema-aware translation of instance documents encoded in XML to
    JSON and vice-versa. See
    [XmlJson](https://github.com/mbj4668/pyang/wiki/XmlJson).
  * Plugin framework for simple development of other outputs, such as
    code generation.


---
