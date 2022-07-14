import Base: show

using Libdl

push!(DL_LOAD_PATH, "lib/")

include("sym.jl")

const AST_NODE_FLAG_ATOM = Csize_t(0)
const AST_NODE_FLAG_EXPR = Csize_t(1)

struct C_ast_node_t # C: ast_node_t *
    ptr :: Culonglong
end

@enum C_ast_atom_head_t begin
    AST_ATOM_HEAD_NIL = 0
    AST_ATOM_HEAD_SYM = 1
    AST_ATOM_HEAD_INT = 2
end

struct C_ast_atom_body_t
    len :: Csize_t
    str :: Cstring
end

struct C_ast_atom_t
    head :: C_ast_atom_head_t
    body :: C_ast_atom_body_t
end

@enum C_ast_expr_head_t begin
    AST_EXPR_HEAD_INVOKE = 0
end

struct C_ast_expr_body_t
    len :: Csize_t
    cap :: Csize_t
    vec :: Ptr{C_ast_node_t}
end

struct C_ast_expr_t
    head :: C_ast_expr_head_t
    body :: C_ast_expr_body_t
end

##

abstract type jet_node_t end

abstract type jet_atom_t <: jet_node_t end
abstract type jet_expr_t <: jet_node_t end

struct jet_nil_t <: jet_atom_t end

struct jet_sym_t <: jet_atom_t
    val :: UInt
end

struct jet_int_t <: jet_atom_t
    val :: UInt
end

struct jet_scope_t <: jet_expr_t
    vec :: Vector{jet_node_t}
    ret :: jet_node_t
end

abstract type jet_fcall_t <: jet_expr_t end

struct jet_dcall_t <: jet_fcall_t
    func :: jet_sym_t
    args :: Vector{jet_node_t}
end

struct jet_icall_t <: jet_fcall_t
    func :: jet_node_t
    args :: Vector{jet_node_t}
end

function show(io :: IO, sym :: jet_sym_t)
    print(io, "__$(sym.val)__")
end

function show(io :: IO, icall :: jet_icall_t)
    println(io, "<expr:icall>")
    println(io, icall.func)

    for arg in icall.args
        println(io, arg)
    end
end

function instantiate(atom :: C_ast_atom_t)
    if atom.head == AST_ATOM_HEAD_NIL
        return jet_nil_t()
    elseif atom.head == AST_ATOM_HEAD_SYM
        return jet_sym_t(jet_get_sym(atom.body.str))
    elseif atom.head == AST_ATOM_HEAD_INT
        return jet_int_t(parse(Int, unsafe_string(atom.body.str)))
    end
end

function instantiate(expr :: C_ast_expr_t)
    if expr.head == AST_EXPR_HEAD_INVOKE
        data = unsafe_wrap(Vector{C_ast_node_t}, expr.body.vec, expr.body.len, own = true)

        data = map(instantiate, data)

        func = first(data)
        args = data[2:end]
        #= 
        func = instantiate(unsafe_load(expr.body.vec))
        args = Vector{jet_node_t}()

        sizehint!(args, expr.body.len - 1)

        for x in 2:expr.body.len
            ptr = unsafe_load(expr.body.vec, x)
            push!(args, instantiate(ptr))
        end
        =#

        return jet_icall_t(func, args)
    end
end

function instantiate(node :: C_ast_node_t)
    flag = node.ptr & one(Csize_t)
    uptr = node.ptr & ~one(Csize_t)

    if flag == AST_NODE_FLAG_ATOM
        atom = reinterpret(Ptr{C_ast_atom_t}, uptr)
        instantiate(unsafe_load(atom))
    elseif flag == AST_NODE_FLAG_EXPR
        expr = reinterpret(Ptr{C_ast_expr_t}, uptr)
        instantiate(unsafe_load(expr))
    end
end

##

function ast_parse_all()
    instantiate(@ccall :libpcc.pcc_parse_all()::C_ast_node_t)
end

