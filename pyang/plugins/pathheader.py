# -- coding: utf-8 --

"""PathHeader output plugin

Compatible with RFC 8340.

Idea copied from libsmi.
"""

import optparse
import re
import sys

from pyang import plugin
from pyang import statements


def pyang_plugin_init():
    print('pathheader pyang_plugin_init')
    plugin.register_plugin(PathHeader())


class PathHeader(plugin.PyangPlugin):
    def __init__(self):
        print('pathheader init')
        plugin.PyangPlugin.__init__(self, 'pathheader')

    def add_output_format(self, fmts):
        self.multiple_modules = True
        fmts['pathheader'] = self

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
    # 遍历所有包含的module()
    for module in modules:
        printed_header = False
        
        def print_header():
            bstr = ""
            b = module.search_one('belongs-to')
            if b is not None:
                bstr = " (belongs-to %s)" % b.arg
            fd.write("// %s: %s%s\n" % (module.keyword, module.arg, bstr))
            printed_header = True

        # statements.data_definition_keywords:
        # ['container', 'leaf', 'leaf-list', 'list', 'choice', 'anyxml', 'anydata', 'uses', 'augment']
        # 查看这个module中是否包含了以上节点，并保存在chs中
        chs = [ch for ch in module.i_children
               if ch.keyword in statements.data_definition_keywords]
        if path is not None and len(path) > 0:
            chs = [ch for ch in chs if ch.arg == path[0]]
            chpath = path[1:]
        else:
            chpath = path

        # 将这个module中的每一个首级container\leaf\list等节点传入print_children（container等容器节点已经带有孩子信息）
        if len(chs) > 0:
            if not printed_header:
                print_header()
                printed_header = True
            print_children(chs, module, fd, '', chpath, 'data', depth, llen,
                           ctx.opts.tree_no_expand_uses,
                           prefix_with_modname=ctx.opts.modname_prefix)

        mods = [module]
        for i in module.search('include'):
            subm = ctx.get_module(i.arg)
            if subm is not None:
                mods.append(subm)
        section_delimiter_printed = False
        for m in mods:
            for augment in m.search('augment'):
                if (hasattr(augment.i_target_node, 'i_module') and
                        augment.i_target_node.i_module not in modules + mods):
                    if not section_delimiter_printed:
                        fd.write('\n')
                        section_delimiter_printed = True
                    # this augment has not been printed; print it
                    if not printed_header:
                        print_header()
                        printed_header = True
                    print_path("  augment", ":", augment.arg, fd, llen)
                    mode = 'augment'
                    if augment.i_target_node.keyword == 'input':
                        mode = 'input'
                    elif augment.i_target_node.keyword == 'output':
                        mode = 'output'
                    elif augment.i_target_node.keyword == 'notification':
                        mode = 'notification'
                    print_children(augment.i_children, m, fd,
                                   '  ', path, mode, depth, llen,
                                   ctx.opts.tree_no_expand_uses,
                                   prefix_with_modname=ctx.opts.modname_prefix)

        rpcs = [ch for ch in module.i_children
                if ch.keyword == 'rpc']
        rpath = path
        if path is not None:
            if len(path) > 0:
                rpcs = [rpc for rpc in rpcs if rpc.arg == path[0]]
                rpath = path[1:]
            else:
                rpcs = []
        if len(rpcs) > 0:
            if not printed_header:
                print_header()
                printed_header = True
            fd.write("\n  rpcs:\n")
            
            print_children(rpcs, module, fd, '  ', rpath, 'rpc', depth, llen,
                           ctx.opts.tree_no_expand_uses,
                           prefix_with_modname=ctx.opts.modname_prefix)

        notifs = [ch for ch in module.i_children
                  if ch.keyword == 'notification']
        npath = path
        if path is not None:
            if len(path) > 0:
                notifs = [n for n in notifs if n.arg == path[0]]
                npath = path[1:]
            else:
                notifs = []
        if len(notifs) > 0:
            if not printed_header:
                print_header()
                printed_header = True
            fd.write("\n  notifications:\n")
            print_children(notifs, module, fd, '  ', npath,
                           'notification', depth, llen,
                           ctx.opts.tree_no_expand_uses,
                           prefix_with_modname=ctx.opts.modname_prefix)

        if ctx.opts.tree_print_groupings:
            section_delimiter_printed = False
            for m in mods:
                for g in m.search('grouping'):
                    if not printed_header:
                        print_header()
                        printed_header = True
                    if not section_delimiter_printed:
                        fd.write('\n')
                        section_delimiter_printed = True
                    fd.write("  grouping %s\n" % g.arg)
                    print_children(g.i_children, m, fd,
                                   '  ', path, 'grouping', depth, llen,
                                   ctx.opts.tree_no_expand_uses,
                                   prefix_with_modname=ctx.opts.modname_prefix)

        if ctx.opts.tree_print_yang_data:
            yds = module.search(('ietf-restconf', 'yang-data'))
            if len(yds) > 0:
                if not printed_header:
                    print_header()
                    printed_header = True
                section_delimiter_printed = False
                for yd in yds:
                    if not section_delimiter_printed:
                        fd.write('\n')
                        section_delimiter_printed = True
                    fd.write("  yang-data %s:\n" % yd.arg)
                    print_children(yd.i_children, module, fd, '  ', path,
                                   'yang-data', depth, llen,
                                   ctx.opts.tree_no_expand_uses,
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

# print_children在printnode中被递归调用
# i_children一个module下的其中一个孩子节点，其中container或者list等容器节点下的所有孩子节点也都包括着
# i_children[-1] : 递归调用打印孩子节点的最后一个节点
# children.arg 当前遍历到的节点的名称
# prefix : 前序节点的路径
def print_children(i_children, module, fd, prefix, path, mode, depth,
                   llen, no_expand_uses, width=0, prefix_with_modname=False):
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
        if ((ch.keyword == 'input' or ch.keyword == 'output') and
                len(ch.i_children) == 0):
            pass
        else:
            # 无论是否遍历到最后一个节点，#define名称必须是全路径
            if (ch == i_children[-1] 
                or (i_children[-1].keyword == 'output' 
                and len(i_children[-1].i_children) == 0)):
                if(len(ch.arg) > 15):
                    newprefix = prefix.upper() + '_' + ch.arg[0:15].upper()
                else:
                    newprefix = prefix.upper() + '_' + ch.arg.upper()
            else:
                if(len(ch.arg) > 15):
                    newprefix = prefix.upper() + '_' + ch.arg[0:15].upper()
                else:
                    newprefix = prefix.upper() + '_' + ch.arg.upper()

            # 将yang模型中的中划线删除
            newprefix = newprefix.replace('-','')
            if (prefix is not None):
                #print('pre node is ' + prefix)
                ch.prenode = prefix
            
            # 输出当前节点
            print_node(ch, module, fd, newprefix, path, mode, depth, llen,
                       no_expand_uses, width,
                       prefix_with_modname=prefix_with_modname)

# s.i_module.i_modulename : 当前节点所属的module名称
# s.arg : 当前节点的名称
# s.keyword : 当前节点的类型
# module : 当前遍历到的module
# get_flags_str(s, mode) : 当前节点的读写权限
# t = get_typename(s, prefix_with_modname) : 当前节点的数据类型
def print_node(s, module, fd, prefix, path, mode, depth, llen,
               no_expand_uses, width, prefix_with_modname=False):

    line = (prefix[1:])
    line = '#define ' + line

    brcol = len(line) + 4

    # 打印所有节点的全路径
    # print(s.prenode.replace('_','/').lower() + '/' + s.arg)
    prenodetree = s.prenode.replace('_','/').lower()

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

    # 以下组合一行的宏定义值
    if s.keyword == 'list' or s.keyword == 'container':
         line += " \"%s/%s\" " % (prenodetree, name)

    elif s.keyword == 'choice':
        m = s.search_one('mandatory')
        if m is None or m.arg == 'false':
            line += flags + ' (' + name + ')?'
        else:
            line += flags + ' (' + name + ')'
    
    elif s.keyword == 'case':
        line += ':(' + name + ')'
        brcol += 1
    
    else:
        t = get_typename(s, prefix_with_modname)
        line += " \"%s/%s\" " % (prenodetree, name)

    # if s.keyword == 'list':
    #     if s.search_one('key') is not None:
    #         keystr = " [%s]" % re.sub('\s+', ' ', s.search_one('key').arg)
    #         if (llen is not None and
    #                 len(line) + len(keystr) > llen):
    #             fd.write(line + '\n')
    #             line = prefix + ' ' * (brcol - len(prefix))
    #         line += keystr
    #     else:
    #         line += " []"

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
                           llen, no_expand_uses, width - 3,
                           prefix_with_modname=prefix_with_modname)
        else:
            print_children(chs, module, fd, prefix, path, mode, depth, llen,
                           no_expand_uses,
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
