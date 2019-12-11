# -- coding: utf-8 --

"""Tree output plugin

Compatible with RFC 8340.

Idea copied from libsmi.
"""

import optparse
import re
import sys

from pyang import plugin
from pyang import statements

def pyang_plugin_init():
    plugin.register_plugin(MoXMLPlugin())


class MoXMLPlugin(plugin.PyangPlugin):
    def __init__(self):
        plugin.PyangPlugin.__init__(self, 'moxml')

    def add_output_format(self, fmts):
        self.multiple_modules = True
        fmts['moxml'] = self

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


all_typedef = {}  # 所有文件中的typedef {def_name, (original_type, range_str)}
xmlheader = '<module name="flexoran-oam-modules" \n\
  xmlns="urn:ietf:params:xml:ns:yang:yin:1" \n\
  xmlns:modules="http://certusnet.com.cn/flexoran/oam/flexoran-oam-modules">\n\
  <namespace uri="http://certusnet.com.cn/flexoran/oam/flexoran-oam-modules"/>\n\
  <prefix value="modules"/>\n\
  <organization>\n\
    <text>CertusNet</text>\n\
  </organization>\n\
  <description>\n\
    <text>This module defines modules configuration.</text>\n\
  </description> \n'

# modules：输入文件的所有包含的module
def emit_tree(ctx, modules, fd, depth, llen, path):
    print('emit_tree: ')
    get_typedef_value(ctx)
    
    printed_header = False
    for module in modules:

        def print_header():
            fd.write(xmlheader)
            printed_header = True

        chs = [ch for ch in module.i_children
               if ch.keyword in statements.data_definition_keywords]
        if path is not None and len(path) > 0:
            chs = [ch for ch in chs if ch.arg == path[0]]
            chpath = path[1:]
        else:
            chpath = path

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

    fd.write('</module>')


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


# i_children一个container或者list等容器节点下的所有孩子节点
# - i_children[-1] : 递归调用打印孩子节点的最后一个节点
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

    for ch in i_children:
        if ((ch.keyword == 'input' or ch.keyword == 'output') and
                len(ch.i_children) == 0):
            pass
        else:
            print('print_children ' + ch.arg)
            if (ch == i_children[-1] or
                (i_children[-1].keyword == 'output' and
                 len(i_children[-1].i_children) == 0)):
                # the last test is to detect if we print input, and the
                # next node is an empty output node; then don't add the |
                newprefix = prefix + '/' + ch.arg
            else:
                newprefix = prefix + '/' + ch.arg

            if ch.keyword == 'input':
                mode = 'input'
            elif ch.keyword == 'output':
                mode = 'output'

            #newprefix = prefix + '   '
            if (prefix is not None):
                #print('pre node is ' + prefix)
                ch.prenode = prefix

            print_node(ch, module, fd, newprefix, path, mode, depth, llen,
                       no_expand_uses, width,
                       prefix_with_modname=prefix_with_modname)


def print_node(s, module, fd, prefix, path, mode, depth, llen,
               no_expand_uses, width, prefix_with_modname=False):

    if s.i_module.i_modulename == module.i_modulename:
        name = s.arg
    else:
        if prefix_with_modname:
            name = s.i_module.i_modulename + ':' + s.arg
        else:
            name = s.i_module.i_prefix + ':' + s.arg

    line = "  <%s>\n" % (name)

    sfmt = "    <{0}>{1}</{0}>\n"
    content = ''

    item = sfmt.format("para_name", name)
    content += item

    item = sfmt.format("type", s.keyword)
    content += item

    nsstr = ''
    ns = s.i_orig_module.search_one('namespace')
    if ns is not None:
        nsstr = ns.arg
    item = sfmt.format("namespace", nsstr)
    content += item

    item = sfmt.format("prefix", s.i_orig_module.i_prefix)
    content += item

    item = sfmt.format("modulename", s.i_orig_module.i_modulename)
    content += item

    if s.keyword == 'leaf-list' or s.keyword == 'leaf':
        range_str = ''
        data_type = get_typename(s, prefix_with_modname)
        if data_type.find(':') != -1: #typedef
            data_type = data_type.split(':')[-1]
        if data_type in all_typedef:
            data_type, range_str = all_typedef[data_type]
        item = sfmt.format("data_type", data_type)
        content += item

        des = s.search_one('description')
        if des is not None:
            item = sfmt.format("para_en_desc", des.arg)
        else:
            item = sfmt.format("para_en_desc", "")
        content += item

        item = sfmt.format("para_ch_desc", "")
        content += item

        item = sfmt.format("is_dynamic_para", "NO")
        content += item

        unit_val = s.search_one('units')
        if unit_val is not None:
            item = sfmt.format("unit", unit_val.arg)
        else:
            item = sfmt.format("unit", "")
        content += item

        if range_str == '':
            range_str = get_typerange(s)
        item = sfmt.format("memory_range", range_str)
        content += item

        dft_val = s.search_one('default')
        if dft_val is not None:
            item = sfmt.format("dft_val", dft_val.arg)
        else:
            item = sfmt.format("dft_val", "")
        content += item

        para_path = "%s/%s"% (s.prenode, name)
        item = sfmt.format("para_path", para_path)
        content += item

        item = sfmt.format("operation", "MOD")
        content += item
    elif s.keyword == 'container':
        para_path = "%s/%s"% (s.prenode, name)
        item = sfmt.format("MO_PATH", para_path)
        content += item

        item = sfmt.format("MO_KEY_LIST", "")
        content += item

        item = sfmt.format("MO_OPERATION", "MOD")
        content += item
    elif s.keyword == 'list':
        para_path = "%s/%s"% (s.prenode, name)
        item = sfmt.format("MO_PATH", para_path)
        content += item

        inst_num =''
        num = s.search_one('min-elements')
        if num is not None:
            inst_num = num.arg + '...'
        num = s.search_one('max-elements')
        if num is not None:
            if inst_num == '':
                inst_num += num.arg
            else:
                inst_num +=  '...' + num.arg
        item = sfmt.format("MO_INST_NUM", inst_num)
        content += item

        keystr = ''
        if s.search_one('key') is not None:
            keystr = re.sub('\s+', ' ', s.search_one('key').arg)
        item = sfmt.format("MO_KEY_LIST", keystr)
        content += item

        item = sfmt.format("MO_OPERATION", "ADD RMV MOD")
        content += item

    line += content

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
        #line += fstr

    line += "  </%s>\n" % (name)
    fd.write(line)

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

def get_typedef_value(ctx):
    for typedefmd in ctx.modules:
        md = ctx.modules[typedefmd]
        for tdname in md.i_typedefs:
            td = md.i_typedefs[tdname]
            if td.keyword == "typedef":
                for ch in td.substmts:
                    range_str = ''
                    if ch.keyword != "type":
                        continue
                    elif ch.arg == "enumeration":
                        range_str = ','.join([e.arg for e in ch.substmts])
                    else:
                        p = ch.search_one('range')
                        if p is not None:
                            range_str = p.arg
                    # print(td.arg + ' == ' + ch.arg + ' ==>' + range_str)
                    all_typedef[td.arg] = (ch.arg, range_str)

    # for k, v in all_typedef.items():
        # print(k,v)


def get_typerange(s):
    t = s.search_one('type')
    if t is not None:
        p = t.search_one('range')
        if p is not None:
            return p.arg

    return ''