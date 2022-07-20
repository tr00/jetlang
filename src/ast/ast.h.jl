module AST

export jet_node_t, jet_atom_t, jet_expr_t

export jet_nil_t, jet_sym_t, jet_int_t

export jet_call_t

## abstract types

abstract type jet_node_t end

abstract type jet_atom_t <: jet_node_t end

abstract type jet_expr_t <: jet_node_t end

## atom types

struct jet_nil_t <: jet_atom_t end

struct jet_sym_t <: jet_atom_t
    sym :: Csymbol
end

struct jet_int_t <: jet_atom_t 
    val :: Cint
end

## expr types

struct jet_call_t <: jet_expr_t end

## constructors

jet_sym_t(str :: String) = jet_sym_t(Csymbol(str))
jet_sym_t(str :: Cstring, len :: Csize_t) = jet_sym_t(Csymbol(str, len))


end
