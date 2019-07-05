# -- coding: utf-8 --

"""xTypeDef output plugin

Compatible with RFC 8340.

Idea copied from libsmi.
"""

import optparse
import re
import sys

from pyang import plugin
from pyang import statements


def pyang_plugin_init():
    #print('xtypedef pyang_plugin_init')
    plugin.register_plugin(xTypeDef())


class xTypeDef(plugin.PyangPlugin):
    def __init__(self):
        plugin.PyangPlugin.__init__(self, 'xtypedef')

    # 向主控模块注册 xtypedef
    def add_output_format(self, fmts):
        self.multiple_modules = True
        fmts['xtypedef'] = self

    def setup_fmt(self, ctx):
        ctx.implicit_errors = False

    def emit(self, ctx, modules, fd):
        if ctx.opts.tree_path is not None:
            path = ctx.opts.tree_path.split('/')
            if path[0] == '':
                path = path[1:]
        else:
            path = None
        emit_tree(ctx, modules, fd, ctx.opts.tree_depth,
                  ctx.opts.tree_line_length, path)

# modules: the yang module
# fd: 输出文件句柄


def emit_tree(ctx, modules, fd, depth, llen, path):
    alreadyGen = []

    for module in modules:
        mod_name = module.arg.replace(
            'certus-5gnr-du-', 'gnb_du_oam_agent_').replace('-', '_')
        headerline = "/*\n" + " * filename: " + mod_name + ".h \n"
        headerline += " * This header file contains implementation of OAM Agent RConfD Generate by Tools \n"
        headerline += "*/ \n\n"

        headerline += "#ifndef " + "__" + mod_name.upper() + "__\n"
        headerline += "#define " + "__" + mod_name.upper() + "__\n\n"

        headerline += """#include "gnb_du_oam_agent_rcfd_cell_types.h"\n\n"""

        headerline += "namespace gnb_du \n"
        headerline += "{\n"
        headerline += "namespace rcfd_cell\n"
        headerline += "{\n"

        fd.write(headerline + "\n")
        #print(module.keyword)

        # 第一次遍历 将所有的grouping都生成typedef
        for groupname in module.i_groupings:
            chs_ctn = [ch for ch in module.i_groupings[groupname].i_children
                       if ch.keyword in statements.mem_definition_keywords]
            if path is not None and len(path) > 0:
                chs_ctn = [ch for ch in chs_ctn if ch.arg == path[0]]
                chpath = path[1:]
            else:
                chpath = path

            if len(chs_ctn) > 0:
                line = "typedef struct struct" + \
                    groupname.replace("-", "").title() + "\n{\n"
                fd.write(line)
                print_grouping(chs_ctn, module, fd, '', chpath, 'data', depth, llen,
                               ctx.opts.tree_no_expand_uses, 0, alreadyGen,
                               prefix_with_modname=ctx.opts.modname_prefix)
                line = "} " + get_struct_name(groupname) + ";\n\n"
                fd.write(line)
                alreadyGen.append(groupname)

        # 第二遍循环 输出所有首级container和list的typedef
        for groupname in module.i_groupings:
            chs = [ch for ch in module.i_groupings[groupname].i_children
                   if ch.keyword in statements.type_definition_keywords
                   and ch.arg not in alreadyGen]
            if path is not None and len(path) > 0:
                chs = [ch for ch in chs if ch.arg == path[0]]
                chpath = path[1:]
            else:
                chpath = path
            if len(chs) > 0:
                print_children(chs, module, fd, '', chpath, 'data', depth, llen,
                               ctx.opts.tree_no_expand_uses, 0, alreadyGen,
                               prefix_with_modname=ctx.opts.modname_prefix)

        # 第三次遍历 则遍历所有节点，找出非首级的container和list
        for groupname in module.i_groupings:
            chs_ctn = [ch for ch in module.i_groupings[groupname].i_children
                       if ch.keyword in statements.data_definition_keywords]

            if path is not None and len(path) > 0:
                chs_ctn = [ch for ch in chs_ctn if ch.arg == path[0]]
                chpath = path[1:]
            else:
                chpath = path

            if len(chs_ctn) > 0:
                print_children2(chs_ctn, module, fd, '', chpath, 'data', depth, llen,
                                ctx.opts.tree_no_expand_uses, 0, alreadyGen,
                                prefix_with_modname=ctx.opts.modname_prefix)

        # 第四次遍历 将与submodule与group名称相同的grouping取出，并生成成员变量
        classname = mod_name.replace('gnb_du_', '')
        classline = "class " + classname + \
            " : public allocator\n{\nprivate:\n"
        fd.write(classline)
        for groupname in module.i_groupings:
            if groupname != module.arg.replace('certus-5gnr-du-', ''):
                continue
            else:
                chs_ctn = [ch for ch in module.i_groupings[groupname].i_children
                           if ch.keyword in statements.mem_definition_keywords]
            if len(chs_ctn) > 0:
                print_mem(chs_ctn, module, fd, '', chpath, 'data', depth, llen,
                          ctx.opts.tree_no_expand_uses, 0, alreadyGen,
                          prefix_with_modname=ctx.opts.modname_prefix)

        fd.write("\n")

        alreadyGen = []
        # 第六遍循环 输出所有首级container和list的read_function
        for groupname in module.i_groupings:
            if groupname != module.arg.replace('certus-5gnr-du-', ''):
                continue
            else:
                chs_func_1 = [ch for ch in module.i_groupings[groupname].i_children
                              if ch.keyword in statements.type_definition_keywords]

            if len(chs_func_1) > 0:
                print_children_read_func_first(chs_func_1, module, fd, '', chpath, 'data', depth, llen,
                                               ctx.opts.tree_no_expand_uses, 0, alreadyGen,
                                               prefix_with_modname=ctx.opts.modname_prefix)

        # 第七次遍历 所有grouping的read_function
        for groupname in module.i_groupings:
            if groupname == module.arg.replace('certus-5gnr-du-', ''):
                continue
            print_read_grp_func(groupname, module, fd, '', chpath, 'data', depth, llen,
                                ctx.opts.tree_no_expand_uses, 0, alreadyGen,
                                prefix_with_modname=ctx.opts.modname_prefix)

        classline = "\npublic:\n"
        classline += "    " + classname + "(XCONFD_YANGTREE_T* yt) {}\n"
        classline += "    vritual ~" + classname + "() {}\n};\n\n"
        classline += "} //" + "end of namespace rcfd_cell\n"
        classline += "} //" + "end of namespace gnb_du\n"
        classline += "#endif /* __" + mod_name.upper() + "__ */"

        fd.write(classline)


def unexpand_uses(i_children):
    res = []
    uses = []
    for ch in i_children:
        if hasattr(ch, 'i_uses'):
            # take first from i_uses, which means "closest" grouping
            g = ch.i_uses[0].arg
            if g not in uses:
                # first node from this uses
                uses.append(g)
                res.append(ch.i_uses[0])
        else:
            res.append(ch)
    return res


def print_path(pre, post, path, fd, llen):
    def print_comps(pre, p, is_first):
        line = pre + '/' + p[0]
        p = p[1:]
        if len(line) > llen:
            # too long, print it anyway; it won't fit next line either
            pass
        else:
            while len(p) > 0 and len(line) + 1 + len(p[0]) <= llen:
                if len(p) == 1 and len(line) + 1 + len(p[0]) + len(post) > llen:
                    # if this is the last component, ensure 'post' fits
                    break
                line += '/' + p[0]
                p = p[1:]
        if len(p) == 0:
            line += post
        line += '\n'
        fd.write(line)
        if len(p) > 0:
            if is_first:
                pre = ' ' * (len(pre) + 2)  # indent next line
            print_comps(pre, p, False)

    line = pre + ' ' + path + post
    if llen is None or len(line) <= llen:
        fd.write(line + '\n')
    else:
        p = path.split('/')
        if p[0] == '':
            p = p[1:]
        pre += " "
        print_comps(pre, p, True)


def refine_type_name(typename):
    if typename.find(':') > 0:
        typename = typename[(typename.find(':') + 1):]
        typename = get_struct_name(typename)
        return typename

    if typename == "string":
        return "std::string"
    if typename == "int8":
        return "int8_t"
    if typename == "int16":
        return "int16_t"
    if typename == "int32":
        return "int32_t"
    if typename == "int64":
        return "uint64_t"
    if typename == "uint8":
        return "uint8_t"
    if typename == "uint16":
        return "uint16_t"
    if typename == "uint32":
        return "uint32_t"
    if typename == "uint64":
        return "uint64_t"
    if typename == "empty":
        return "bool"

    return typename


def get_struct_name(struname):
    ret = ""
    temp = struname.split('-')
    for each in temp:
        ret += each.title()

    return ret


def print_grouping(i_children, module, fd, prefix, path, mode, depth,
                   llen, no_expand_uses, level, alreadyGen, width=0, prefix_with_modname=False):
    for child in i_children:
        if ((child.keyword == "container")):
            line = "    %s %s; " % (get_struct_name(child.arg),
                                    child.arg.replace('-', '_'))
        elif ((child.keyword == "list")):
            line = "    std::vector<std::shared_ptr<" + \
                get_struct_name(child.arg) + "> " + \
                child.arg.replace('-', '_') + ";"
        elif child.keyword == "leaf-list":
            line = "    std::vector<std::shared_ptr<" + \
                refine_type_name(get_typename(
                    child, prefix_with_modname)) + ">> " + child.arg.replace('-', '_') + ";"
        else:
            line = "    %s %s; " % (refine_type_name(
                (get_typename(child, prefix_with_modname))), child.arg.replace('-', '_'))

        fd.write(line + '\n')


def print_mem(i_children, module, fd, prefix, path, mode, depth,
              llen, no_expand_uses, level, alreadyGen, width=0, prefix_with_modname=False):
    for child in i_children:
        if ((child.keyword == "container")):
            opt = [temp for temp in child.substmts
                   if temp.arg == "optional"]
            if (len(opt) > 0):
                line = "    std::shared_ptr<" + \
                    get_struct_name(child.arg) + "> " + \
                    child.arg.replace('-', '_') + "_;"
            else:
                line = "    %s %s_; " % (get_struct_name(child.arg),
                                         child.arg.replace('-', '_'))
        elif ((child.keyword == "list")):
            line = "    std::vector<std::shared_ptr<" + \
                get_struct_name(child.arg) + "> " + \
                child.arg.replace('-', '_') + "_;"
        elif child.keyword == "leaf-list":
            line = "    std::vector<std::shared_ptr<" + \
                refine_type_name(get_typename(
                    child, prefix_with_modname)) + ">> " + child.arg.replace('-', '_') + "_;"
        else:
            line = "    %s %s_; " % (refine_type_name(
                (get_typename(child, prefix_with_modname))), child.arg.replace('-', '_'))

        fd.write(line + '\n')

# print_children在printnode中被递归调用
# i_children一个module下的其中一个孩子节点，其中container或者list等容器节点下的所有孩子节点也都包括着
# i_children[-1] : 递归调用打印孩子节点的最后一个节点
# children.arg 当前遍历到的节点的名称
# prefix : 前序节点的路径


def print_children(i_children, module, fd, prefix, path, mode, depth,
                   llen, no_expand_uses, level, alreadyGen, width=0, prefix_with_modname=False):

    if depth == 0:
        if i_children:
            fd.write(prefix + '     ...\n')
        return

    def get_width(w, chs):
        for ch in chs:
            if ch.keyword in ['choice', 'case']:
                nlen = 3 + get_width(0, ch.i_children)
            else:
                if ch.i_module.i_modulename == module.i_modulename:
                    nlen = len(ch.arg)
                else:
                    nlen = len(ch.i_module.i_prefix) + 1 + len(ch.arg)
            if nlen > w:
                w = nlen
        return w

    if no_expand_uses:
        i_children = unexpand_uses(i_children)

    if width == 0:
        width = get_width(0, i_children)

    # 遍历这个孩子节点中的所有孩子节点
    for ch in i_children:
        endofvec = False
        if ((ch.keyword == 'input' or ch.keyword == 'output') and
                len(ch.i_children) == 0):
            pass
        else:
            # 当遍历到该节点所有子节点的最后一个节点时
            if (ch == i_children[-1]
                or (i_children[-1].keyword == 'output'
                    and len(i_children[-1].i_children) == 0)):
                newprefix = prefix.upper() + '__' + ch.arg.upper()
                if level != 0:
                    endofvec = True
            else:
                newprefix = prefix.upper() + '__' + ch.arg.upper()

            # 将yang模型中的中划线删除
            newprefix = newprefix.replace('-', '_')

            # 记录该节点的父节点
            if (prefix is not None):
                ch.prenode = prefix

            # 输出当前节点
            print_node(ch, module, fd, newprefix, path, mode, depth, llen,
                       no_expand_uses, level, width, endofvec, alreadyGen,
                       prefix_with_modname=prefix_with_modname)

# s.i_module.i_modulename : 当前节点所属的module名称
# s.arg : 当前节点的名称
# s.keyword : 当前节点的类型
# module : 当前遍历到的module
# get_flags_str(s, mode) : 当前节点的读写权限
# t = get_typename(s, prefix_with_modname) : 当前节点的数据类型


def print_node(s, module, fd, prefix, path, mode, depth, llen,
               no_expand_uses, level, width, endofvec, alreadyGen, prefix_with_modname=False):

    line = ""
    endflag = False
    #line += str(level) + "---"

    strname = get_struct_name(str(s.parent.arg))
    # 第一次遍历的时候，先将所有的顶级的container或者list写出来
    if ((s.keyword == "container") and level == 0):
        line += "typedef struct struct" + \
            s.arg.replace("-", "").title() + "\n{"

    if ((s.keyword == "list") and level == 0):
        line += "typedef struct struct" + \
            s.arg.replace("-", "").title() + "\n{"

    # 检查遍历到的节点是否是当前的module
    if s.i_module.i_modulename == module.i_modulename:
        name = s.arg
    else:
        if prefix_with_modname:
            name = s.i_module.i_modulename + ':' + s.arg
        else:
            name = s.i_module.i_prefix + ':' + s.arg

    # t : 当前节点的数据类型
    t = get_typename(s, prefix_with_modname)

    # 以下组合一行的宏定义值
    if (s.keyword == 'list' or s.keyword == 'container') and (level == 0):
        line += ""
    # 第一次遍历，只要这个container不是第一级，则它就是其他container的节点
    elif ((s.keyword == "container") and level != 0):
        line += "    %s %s; " % (get_struct_name(s.arg),
                                 name.replace('-', '_'))
    elif ((s.keyword == "list" or s.keyword == "leaf-list") and level != 0):
        line += "    std::vector<std::shared_ptr<" + \
            get_struct_name(s.arg) + ">> " + name.replace('-', '_') + ";"
    else:
        if s.parent.keyword == "list" or s.parent.keyword == "container":
            line += "    %s %s; " % (refine_type_name(t),
                                     name.replace('-', '_'))

    # 如果一个container或者list结束了，则结束该typedef
    if endofvec == True and endflag == False:
        line += "\n} " + strname + "; \n"
        alreadyGen.append(s.parent.arg)

    # 将当前的节点写入文件中
    fd.write(line + '\n')

    # 如果当前节点有孩子（不是第一级不再往下遍历了）
    if hasattr(s, 'i_children') and s.keyword != 'uses' and level == 0:
        if depth is not None:
            depth = depth - 1
        chs = s.i_children
        if path is not None and len(path) > 0:
            chs = [ch for ch in chs
                   if ch.arg == path[0]]
            path = path[1:]
        if s.keyword in ['choice', 'case']:
            print_children(chs, module, fd, prefix, path, mode, depth,
                           llen, no_expand_uses, level + 1,  width - 3, alreadyGen,
                           prefix_with_modname=prefix_with_modname)
        else:
            print_children(chs, module, fd, prefix, path, mode, depth, llen,
                           no_expand_uses, level + 1, alreadyGen,
                           prefix_with_modname=prefix_with_modname)


def print_children_read_func_first(i_children, module, fd, prefix, path, mode, depth,
                                   llen, no_expand_uses, level, alreadyGen, width=0, prefix_with_modname=False):
    endofvec = False
    # 遍历这个孩子节点中的所有container或list节点
    for ch in i_children:
        newprefix = (prefix + '__' + ch.arg).replace('-', '_')
        if level != 0:
            endofvec = True

        # 记录该节点的父节点
        if (prefix is not None):
            ch.prenode = prefix

        # 输出当前节点
        print_read_func_first(ch, module, fd, newprefix, path, mode, depth, llen,
                              no_expand_uses, level, width, endofvec, alreadyGen,
                              prefix_with_modname=prefix_with_modname)

# s.i_module.i_modulename : 当前节点所属的module名称
# s.arg : 当前节点的名称
# s.keyword : 当前节点的类型
# module : 当前遍历到的module
# get_flags_str(s, mode) : 当前节点的读写权限
# t = get_typename(s, prefix_with_modname) : 当前节点的数据类型


def print_read_func_first(s, module, fd, prefix, path, mode, depth, llen,
                          no_expand_uses, level, width, endofvec, alreadyGen, prefix_with_modname=False):

    line = ""
    # 第一次遍历的时候，先将所有的顶级的container或者list写出来
    if (level == 0):
        line = "    void read_" + \
            s.arg.replace('-', '_') + "(XCONFD_YANGTREE_T* yt);"
        # 将当前的节点写入文件中
        fd.write(line + '\n')
    
    chs = [ch for ch in s.i_children
            if ch.keyword in ['container', 'list']
            and judge_if_uses_state(s) == 4]
    
    for prt_ch in chs:
        if prt_ch.keyword == "list":
            line = "    void read_" + s.arg.replace('-','_') + "__" + prt_ch.arg.replace('-','_') + "(XCONFD_YANGTREE_T* yt, " + \
                "std::vector<std::shared_ptr<" + get_struct_name(prt_ch.arg) + ">>& " + prt_ch.arg.replace('-', '_') + ");"
            fd.write(line + '\n')
        if prt_ch.keyword == "container":
            line = "    void read_" + s.arg.replace('-','_') + "__" + prt_ch.arg.replace('-','_') + "(XCONFD_YANGTREE_T* yt, " + \
                "" + get_struct_name(prt_ch.arg) + "& " + prt_ch.arg.replace('-', '_') + ");"
            fd.write(line + '\n')


def judge_next_ctn(i_children, level):
    if (i_children.keyword == "container" or i_children.keyword == "list") and level > 0:
        return True

# print_children在printnode中被递归调用
# i_children一个module下的其中一个孩子节点，其中container或者list等容器节点下的所有孩子节点也都包括着
# i_children[-1] : 递归调用打印孩子节点的最后一个节点
# children.arg 当前遍历到的节点的名称
# prefix : 前序节点的路径


def print_children2(i_children, module, fd, prefix, path, mode, depth,
                    llen, no_expand_uses, level, alreadyGen, width=0, prefix_with_modname=False):
    if depth == 0:
        if i_children:
            fd.write(prefix + '     ...\n')
        return

    def get_width(w, chs):
        for ch in chs:
            if ch.keyword in ['choice', 'case']:
                nlen = 3 + get_width(0, ch.i_children)
            else:
                if ch.i_module.i_modulename == module.i_modulename:
                    nlen = len(ch.arg)
                else:
                    nlen = len(ch.i_module.i_prefix) + 1 + len(ch.arg)
            if nlen > w:
                w = nlen
        return w

    if no_expand_uses:
        i_children = unexpand_uses(i_children)

    if width == 0:
        width = get_width(0, i_children)

    # 遍历这个孩子节点中的所有孩子节点
    for ch in i_children:
        endofvec = False
        chs = []

        # 当遍历到该节点所有子节点的最后一个节点时
        if (ch == i_children[-1]
            or (i_children[-1].keyword == 'output'
                and len(i_children[-1].i_children) == 0)):
            newprefix = prefix.upper() + '__' + ch.arg.upper()
            if level != 0:
                endofvec = True
        else:
            newprefix = prefix.upper() + '__' + ch.arg.upper()

        # 将yang模型中的中划线删除
        newprefix = newprefix.replace('-', '_')
        if (prefix is not None):
            #print('pre node is ' + prefix)
            ch.prenode = prefix

        # 查找所有非第一级的container和list
        if ch.keyword == "container" or ch.keyword == "list":
            for cht in ch.i_children:
                if cht.keyword in statements.type_definition_keywords and cht.arg not in alreadyGen:
                    # 此时这个cht一定是一个list或者container
                    # print(cht)
                    line = "typedef struct struct" + \
                        cht.arg.replace("-", "").title() + "\n{"
                    fd.write(line + "\n")
                    print_node2(cht, module, fd, newprefix, path, mode, depth, llen,
                                no_expand_uses, level, width, endofvec, alreadyGen,
                                prefix_with_modname=prefix_with_modname)

        # 遍历孩子节点
        recursive_child(ch, module, fd, newprefix, path, mode, depth, llen,
                        no_expand_uses, level, width, endofvec, alreadyGen,
                        prefix_with_modname=prefix_with_modname)


def print_read_grp_func(groupname, module, fd, prefix, path, mode, depth,
                        llen, no_expand_uses, level, alreadyGen, width=0, prefix_with_modname=False):
    chs = []
    # 打印第一级的grp
    line = "    void read_grp_" + groupname.replace('-', '_') + "(XCONFD_YANGTREE_T* yt, " + get_struct_name(
        groupname) + "& " + groupname.replace('-', '_') + ");"
    # 将当前的节点写入文件中
    fd.write(line + '\n')
    print_read_grp_next_func(groupname, module, fd, 0)


def print_read_grp_next_func(groupname, module, fd, level):

    # 判断这个grouping下是否有container
    chs = [ch for ch in module.i_groupings[groupname].i_children
        if ch.keyword in statements.type_definition_keywords
        and not judge_if_uses_state(module.i_groupings[groupname]) == 2]

    # 循环遍历grouping下所有的container或list，当container或list有孩子为container或list，则需要递归遍历该节点
    for prt_ch in chs:
        #if judge_if_uses_state(prt_ch) == 1:
            line = "    void read_grp_" + groupname.replace('-', '_') + "__" + prt_ch.arg.replace('-', '_') + "(XCONFD_YANGTREE_T* yt, " + get_struct_name(
                judge_if_uses(prt_ch)) + "& " + prt_ch.arg.replace('-','_') + ");"
            fd.write(line + '\n')
    #else:
        #read_grp_next_func(prt_ch, groupname, fd, 0)


# def read_grp_next_func(prt_ch, groupname, fd, level):
    # # 当这个container或list中除了包含uses还包含了其他leaf、container等等
    # chs2 = [ch2 for ch2 in prt_ch.substmts
    #         if ch2.keyword in statements.mem_definition_keywords]
    # # TODO:第二级及其以后的container或list需要处理
    # if (judge_if_uses_state(prt_ch) == 2) and len(chs2) > 0:
    #     print(prt_ch)
    #     chs = [ch for ch in prt_ch.i_children
    #             if ch.keyword in statements.type_definition_keywords]
    #     print_read_grp_next_func(groupname, fd, level + 1)

    # if(len(chs) > 0):
    #     # 将grouping下所有的container或list递归出来
    #     print_grp_children(chs, module, fd, prefix, path, mode, depth,
    #                        llen, no_expand_uses, level, alreadyGen, width=0, prefix_with_modname=False)


def judge_if_uses(child_node):
    child_len = [ch for ch in child_node.substmts
                 if ch.keyword == "uses"]
    child_all_len = [ch2 for ch2 in child_node.substmts]

    if(len(child_len) == 1 and len(child_all_len) == 1):
        return str(child_len[0].arg)
    else:
        return str(child_node.arg)

def judge_if_uses_state(child_node):
    child_len = [ch for ch in child_node.substmts
                 if ch.keyword == "uses"]
    child_all_len = [ch2 for ch2 in child_node.substmts]

    if(len(child_len) == 1 and len(child_all_len) == 1):
        return 1
    elif(len(child_len) == 1):
        return 2
    elif(len(child_len) >= 1):
        return 3
    elif(len(child_len) == 0):
        return 4
    else:
        return 5
    


def print_grp_children(i_children, module, fd, prefix, path, mode, depth,
                       llen, no_expand_uses, level, alreadyGen, width=0, prefix_with_modname=False):

    # 遍历这个孩子节点中的所有孩子节点
    for ch in i_children:
        endofvec = False
        chs = []

        # 当遍历到该节点所有子节点的最后一个节点时
        if (ch == i_children[-1]
            or (i_children[-1].keyword == 'output'
                and len(i_children[-1].i_children) == 0)):
            newprefix = prefix.upper() + '__' + ch.arg.upper()
            if level != 0:
                endofvec = True
        else:
            newprefix = prefix.upper() + '__' + ch.arg.upper()

        # 将yang模型中的中划线删除
        newprefix = newprefix.replace('-', '_')
        if (prefix is not None):
            #print('pre node is ' + prefix)
            ch.prenode = prefix

        # 查找所有非第一级的container和list
        if ch.keyword == "container" or ch.keyword == "list":
            for cht in ch.i_children:
                if cht.keyword in statements.type_definition_keywords and cht.arg not in alreadyGen:
                    # 此时这个cht一定是一个list或者container
                    # print(cht)
                    line = "typedef struct struct" + \
                        cht.arg.replace("-", "").title() + "\n{"
                    fd.write(line + "\n")
                    print_node2(cht, module, fd, newprefix, path, mode, depth, llen,
                                no_expand_uses, level, width, endofvec, alreadyGen,
                                prefix_with_modname=prefix_with_modname)

        # 遍历孩子节点
        recursive_child(ch, module, fd, newprefix, path, mode, depth, llen,
                        no_expand_uses, level, width, endofvec, alreadyGen,
                        prefix_with_modname=prefix_with_modname)


# s.i_module.i_modulename : 当前节点所属的module名称
# s.arg : 当前节点的名称
# s.keyword : 当前节点的类型
# module : 当前遍历到的module
# get_flags_str(s, mode) : 当前节点的读写权限
# t = get_typename(s, prefix_with_modname) : 当前节点的数据类型
def recursive_child(s, module, fd, prefix, path, mode, depth, llen,
                    no_expand_uses, level, width, endofvec, alreadyGen, prefix_with_modname=False):
    # 如果当前节点有孩子
    if hasattr(s, 'i_children') and s.keyword != 'uses':
        if depth is not None:
            depth = depth - 1
        chs = s.i_children
        if path is not None and len(path) > 0:
            chs = [ch for ch in chs
                   if ch.arg == path[0]]
            path = path[1:]
        if s.keyword in ['choice', 'case']:
            print_children2(chs, module, fd, prefix, path, mode, depth,
                            llen, no_expand_uses, level + 1,  width - 3, alreadyGen,
                            prefix_with_modname=prefix_with_modname)
        else:
            print_children2(chs, module, fd, prefix, path, mode, depth, llen,
                            no_expand_uses, level + 1, alreadyGen,
                            prefix_with_modname=prefix_with_modname)


def print_node2(s, module, fd, prefix, path, mode, depth, llen,
                no_expand_uses, level, width, endofvec, alreadyGen, prefix_with_modname=False):

    line = ""
    strname = get_struct_name(str(s.arg))

    for children in s.i_children:
        if ((children.keyword == "container")):
            line = "    %s %s; " % (get_struct_name(children.arg),
                                    children.arg.replace('-', '_'))
        elif ((children.keyword == "list")):
            line = "    std::vector<std::shared_ptr<" + \
                get_struct_name(children.arg) + "> " + \
                children.arg.replace('-', '_') + ";"
        elif children.keyword == "leaf-list":
            line = "    std::vector<std::shared_ptr<" + \
                refine_type_name(get_typename(
                    children, prefix_with_modname)) + ">> " + children.arg.replace('-', '_') + ";"
        else:
            line = "    %s %s; " % (refine_type_name(
                (get_typename(children, prefix_with_modname))), children.arg.replace('-', '_'))

        fd.write(line + '\n')

    line = "} " + strname + "; \n"
    alreadyGen.append(s.parent.arg)

    fd.write(line + '\n')


def get_status_str(s):
    status = s.search_one('status')
    if status is None or status.arg == 'current':
        return '+'
    elif status.arg == 'deprecated':
        return 'x'
    elif status.arg == 'obsolete':
        return 'o'


def get_flags_str(s, mode):
    if mode == 'input':
        return "-w"
    elif s.keyword in ('rpc', 'action', ('tailf-common', 'action')):
        return '-x'
    elif s.keyword == 'notification':
        return '-n'
    elif s.keyword == 'uses':
        return '-u'
    elif s.i_config == True:
        return 'rw'
    elif s.i_config == False or mode == 'output' or mode == 'notification':
        return 'ro'
    else:
        return ''


def get_leafref_path(s):
    t = s.search_one('type')
    if t is not None:
        if t.arg == 'leafref':
            return t.search_one('path')
    else:
        return None


def get_typename(s, prefix_with_modname=False):
    t = s.search_one('type')
    if t is not None:
        if t.arg == 'leafref':
            p = t.search_one('path')
            if p is not None:
                # Try to make the path as compact as possible.
                # Remove local prefixes, and only use prefix when
                # there is a module change in the path.
                target = []
                curprefix = s.i_module.i_prefix
                for name in p.arg.split('/'):
                    if name.find(":") == -1:
                        prefix = curprefix
                    else:
                        [prefix, name] = name.split(':', 1)
                    if prefix == curprefix:
                        target.append(name)
                    else:
                        if prefix_with_modname:
                            if prefix in s.i_module.i_prefixes:
                                # Try to map the prefix to the module name
                                (module_name,
                                 _) = s.i_module.i_prefixes[prefix]
                            else:
                                # If we can't then fall back to the prefix
                                module_name = prefix
                            target.append(module_name + ':' + name)
                        else:
                            target.append(prefix + ':' + name)
                        curprefix = prefix
                return "-> %s" % "/".join(target)
            else:
                # This should never be reached. Path MUST be present for
                # leafref type. See RFC6020 section 9.9.2
                # (https://tools.ietf.org/html/rfc6020#section-9.9.2)
                if prefix_with_modname:
                    if t.arg.find(":") == -1:
                        # No prefix specified. Leave as is
                        return t.arg
                    else:
                        # Prefix found. Replace it with the module name
                        [prefix, name] = t.arg.split(':', 1)
                        # return s.i_module.i_modulename + ':' + name
                        if prefix in s.i_module.i_prefixes:
                            # Try to map the prefix to the module name
                            (module_name, _) = s.i_module.i_prefixes[prefix]
                        else:
                            # If we can't then fall back to the prefix
                            module_name = prefix
                        return module_name + ':' + name
                else:
                    return t.arg
        else:
            if prefix_with_modname:
                if t.arg.find(":") == -1:
                    # No prefix specified. Leave as is
                    return t.arg
                else:
                    # Prefix found. Replace it with the module name
                    [prefix, name] = t.arg.split(':', 1)
                    # return s.i_module.i_modulename + ':' + name
                    if prefix in s.i_module.i_prefixes:
                        # Try to map the prefix to the module name
                        (module_name, _) = s.i_module.i_prefixes[prefix]
                    else:
                        # If we can't then fall back to the prefix
                        module_name = prefix
                    return module_name + ':' + name
            else:
                return t.arg
    elif s.keyword == 'anydata':
        return '<anydata>'
    elif s.keyword == 'anyxml':
        return '<anyxml>'
    else:
        return ''