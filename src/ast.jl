include("sym.jl")

struct C_ast_node_ptr_t
    ptr :: Culonglong
end

function Base.getproperty(node :: C_ast_node_ptr_t, sym :: Symbol)
    getfield(node, sym)
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
    vec :: Ptr{C_ast_node_ptr_t}
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

struct jet_dcall_t <: jet_expr_t
    func :: jet_sym_t
    args :: Vector{jet_node_t}
end

struct jet_icall_t <: jet_expr_t
    func :: jet_node_t
    args :: Vector{jet_node_t}
end

function instantiate(atom :: C_ast_atom_t)
    if atom.head == AST_ATOM_HEAD_NIL
        return jet_nil_t()
    elseif atom.head == AST_ATOM_HEAD_SYM
        
    end
end
