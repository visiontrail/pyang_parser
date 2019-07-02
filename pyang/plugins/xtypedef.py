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
    for module in modules:
        print(module.keyword)
        # 第一遍循环 输出所有的typedef struct
        for groupname in module.i_groupings:
            chs = [ch for ch in module.i_groupings[groupname].i_children
                   if ch.keyword in statements.data_definition_keywords]
            if path is not None and len(path) > 0:
                chs = [ch for ch in chs if ch.arg == path[0]]
                chpath = path[1:]
            else:
                chpath = path

            if len(chs) > 0:
                print_children(chs, module, fd, '', chpath, 'data', depth, llen,
                               ctx.opts.tree_no_expand_uses, 0,
                               prefix_with_modname=ctx.opts.modname_prefix)


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

def get_struct_name(struname):
    ret = ""
    temp = struname.split('-')
    for each in temp:
        ret += each.title()

    return ret

def switch_comm_type(t):
    if t == "string":
        return "std::string"

# print_children在printnode中被递归调用
# i_children一个module下的其中一个孩子节点，其中container或者list等容器节点下的所有孩子节点也都包括着
# i_children[-1] : 递归调用打印孩子节点的最后一个节点
# children.arg 当前遍历到的节点的名称
# prefix : 前序节点的路径
def print_children(i_children, module, fd, prefix, path, mode, depth,
                   llen, no_expand_uses, level, width=0, prefix_with_modname=False):
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
            # 无论是否遍历到最后一个节点，#define名称必须是全路径
            if (ch == i_children[-1] 
                or (i_children[-1].keyword == 'output' 
                and len(i_children[-1].i_children) == 0)):
                newprefix = prefix.upper() + '__' + ch.arg.upper()
                if level != 0:
                    endofvec = True
                    if (ch.keyword == "list"):
                        line = "    std::vector<std::shared_ptr<" + get_struct_name(ch.arg) + "> " + ch.arg.replace('-','_') + ";"
                        fd.write(line + '\n')
            else:
                newprefix = prefix.upper() + '__' + ch.arg.upper()

            # 将yang模型中的中划线删除
            newprefix = newprefix.replace('-','_')
            if (prefix is not None):
                #print('pre node is ' + prefix)
                ch.prenode = prefix
            
            # 输出当前节点
            print_node(ch, module, fd, newprefix, path, mode, depth, llen,
                       no_expand_uses, level, width, endofvec,
                       prefix_with_modname=prefix_with_modname)

# s.i_module.i_modulename : 当前节点所属的module名称
# s.arg : 当前节点的名称
# s.keyword : 当前节点的类型
# module : 当前遍历到的module
# get_flags_str(s, mode) : 当前节点的读写权限
# t = get_typename(s, prefix_with_modname) : 当前节点的数据类型
def print_node(s, module, fd, prefix, path, mode, depth, llen,
               no_expand_uses, level, width, endofvec, prefix_with_modname=False):

    line = ""
    endflag = False

    #line += str(level) + "---"

    strname = get_struct_name(str(s.parent.arg))

    # 当遍历到的节点为list或者container,且为第一级，则认为它是一个可能包含其他list的struct
    if ((s.keyword == "container") and level == 0):
        line += "typedef struct struct" + s.arg.replace("-","") + " {"
    
    if ((s.keyword == "list") and level == 0):
        line += "typedef struct struct" + s.arg.replace("-","") + " {"

    # 检查遍历到的节点是否是当前的module
    if s.i_module.i_modulename == module.i_modulename:
        name = s.arg
    else:
        if prefix_with_modname:
            name = s.i_module.i_modulename + ':' + s.arg
        else:
            name = s.i_module.i_prefix + ':' + s.arg
    
    # flags : 当前节点的读写权限，即ro\rw等等
    flags = get_flags_str(s, mode)
    # t : 当前节点的数据类型
    t = get_typename(s, prefix_with_modname)

    if t.find(':') > 0:
        t = t[(t.find(':') + 1):]
        t = get_struct_name(t)
    
    # 以下组合一行的宏定义值
    if (s.keyword == 'list' or s.keyword == 'container') and (level == 0):
        line += ""
    # 第一次遍历，只要这个container不是第一级，则它就是其他container的节点
    elif ((s.keyword == "container") and level != 0):
        line += "    %s %s; " % (get_struct_name(s.arg), name.replace('-','_'))
    elif ((s.keyword == "list") and level != 0):
        line += "    std::vector<std::shared_ptr<" + get_struct_name(s.arg) + "> " + name.replace('-','_') + ";"
    else:
        if s.parent.keyword == "list" or s.parent.keyword == "container":
            line += "    %s %s; " % (t, name.replace('-','_'))

    features = s.search('if-feature')
    featurenames = [f.arg for f in features]
    if hasattr(s, 'i_augment'):
        afeatures = s.i_augment.search('if-feature')
        featurenames.extend([f.arg for f in afeatures
                             if f.arg not in featurenames])

    if len(featurenames) > 0:
        fstr = " {%s}?" % ",".join(featurenames)
        if (llen is not None and len(line) + len(fstr) > llen):
            fd.write(line + '\n')
            line = prefix + ' ' * (brcol - len(prefix))
        line += fstr

    if endofvec == True and endflag == False :
        line += "\n }" + strname + ";"
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
                           llen, no_expand_uses, level + 1,  width - 3,
                           prefix_with_modname=prefix_with_modname)
        else:
            print_children(chs, module, fd, prefix, path, mode, depth, llen,
                           no_expand_uses, level + 1, 
                           prefix_with_modname=prefix_with_modname)



# print_children在printnode中被递归调用
# i_children一个module下的其中一个孩子节点，其中container或者list等容器节点下的所有孩子节点也都包括着
# i_children[-1] : 递归调用打印孩子节点的最后一个节点
# children.arg 当前遍历到的节点的名称
# prefix : 前序节点的路径
def print_children2(i_children, module, fd, prefix, path, mode, depth,
                   llen, no_expand_uses, level, width=0, prefix_with_modname=False):
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
            # 无论是否遍历到最后一个节点，#define名称必须是全路径
            if (ch == i_children[-1] 
                or (i_children[-1].keyword == 'output' 
                and len(i_children[-1].i_children) == 0)):
                newprefix = prefix.upper() + '__' + ch.arg.upper()
                if level != 0:
                    endofvec = True
                    if (ch.keyword == "list"):
                        line = "    std::vector<std::shared_ptr<" + get_struct_name(ch.arg) + "> " + ch.arg.replace('-','_') + ";"
                        fd.write(line + '\n')
            else:
                newprefix = prefix.upper() + '__' + ch.arg.upper()

            # 将yang模型中的中划线删除
            newprefix = newprefix.replace('-','_')
            if (prefix is not None):
                #print('pre node is ' + prefix)
                ch.prenode = prefix
            
            # 输出当前节点
            print_node(ch, module, fd, newprefix, path, mode, depth, llen,
                       no_expand_uses, level, width, endofvec,
                       prefix_with_modname=prefix_with_modname)


# s.i_module.i_modulename : 当前节点所属的module名称
# s.arg : 当前节点的名称
# s.keyword : 当前节点的类型
# module : 当前遍历到的module
# get_flags_str(s, mode) : 当前节点的读写权限
# t = get_typename(s, prefix_with_modname) : 当前节点的数据类型
def print_node2(s, module, fd, prefix, path, mode, depth, llen,
               no_expand_uses, level, width, endofvec, prefix_with_modname=False):

    line = ""
    endflag = False

    #line += str(level) + "---"

    strname = get_struct_name(str(s.parent.arg))

    if (((s.keyword == "list") or (s.keyword == "container")) and level != 0):
        line += "}" + strname + ";" + "\n"
        endflag = True

    # 当遍历到的节点为list或者container,且为第一级，则认为它是一个可能包含其他list的struct
    if ((s.keyword == "container")):
        line += "typedef struct struct" + s.arg.replace("-","") + " {"
    
    if ((s.keyword == "list")):
        line += "typedef struct struct" + s.arg.replace("-","") + " {"

    # 打印所有节点的全路径
    # print(s.prenode.replace('_','/').lower() + '/' + s.arg)
    prenodetree = s.prenode.replace('__','/').lower()

    # 检查遍历到的节点是否是当前的module
    if s.i_module.i_modulename == module.i_modulename:
        name = s.arg
    else:
        if prefix_with_modname:
            name = s.i_module.i_modulename + ':' + s.arg
        else:
            name = s.i_module.i_prefix + ':' + s.arg
    
    # flags : 当前节点的读写权限，即ro\rw等等
    flags = get_flags_str(s, mode)
    # t : 当前节点的数据类型
    t = get_typename(s, prefix_with_modname)

    if t.find(':') > 0:
        t = t[(t.find(':') + 1):]
        t = get_struct_name(t)
    


    # 以下组合一行的宏定义值
    if s.keyword == 'list' or s.keyword == 'container':
         line += ""
    else:
        if s.parent.keyword == "list" or s.parent.keyword == "container":
            line += "    %s %s; " % (t, name.replace('-','_'))

    features = s.search('if-feature')
    featurenames = [f.arg for f in features]
    if hasattr(s, 'i_augment'):
        afeatures = s.i_augment.search('if-feature')
        featurenames.extend([f.arg for f in afeatures
                             if f.arg not in featurenames])

    if len(featurenames) > 0:
        fstr = " {%s}?" % ",".join(featurenames)
        if (llen is not None and len(line) + len(fstr) > llen):
            fd.write(line + '\n')
            line = prefix + ' ' * (brcol - len(prefix))
        line += fstr

    if endofvec == True and endflag == False :
        line += "\n }" + strname + ";"
    # 将当前的节点写入文件中
    fd.write(line + '\n')

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
            print_children(chs, module, fd, prefix, path, mode, depth,
                           llen, no_expand_uses, level + 1,  width - 3,
                           prefix_with_modname=prefix_with_modname)
        else:
            print_children(chs, module, fd, prefix, path, mode, depth, llen,
                           no_expand_uses, level + 1, 
                           prefix_with_modname=prefix_with_modname)

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
