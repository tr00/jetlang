module AST

include("../sym/sym.h.jl")

import Base: show

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

struct jet_call_t <: jet_expr_t
    args :: Vector{jet_node_t}
end

## constructors

jet_sym_t(str :: String) = jet_sym_t(Csymbol(str))
jet_sym_t(str :: Cstring, len :: Cint) = jet_sym_t(Csymbol(str, len))

jet_call_t(nodes :: jet_node_t ...) = jet_call_t(jet_node_t[nodes...])

## pretty printing

show(io :: IO, ::jet_nil_t) = print(io, "()")

show(io :: IO, sym :: jet_sym_t) = print(io, string(sym.sym))

show(io :: IO, int :: jet_int_t) = print(io, int.val)

show(io :: IO, call :: jet_call_t) = tostring(io, call)

function tostring(io :: IO, nil :: jet_nil_t, cd = 0, md = 8)
    if cd < md
        print(io, ' '^cd, "<atom:nil>")
    end
end

function tostring(io :: IO, int :: jet_int_t, cd = 0, md = 8)
    if cd < md
        print(io, ' '^cd, "<atom:int> $(int.val)")
    end
end

function tostring(io :: IO, sym :: jet_sym_t, cd = 0, md = 8)
    if cd < md
        print(io, ' '^cd, "<atom:sym> $(string(sym))")
    end
end

function tostring(io :: IO, call :: jet_call_t, cd = 0, md = 8)
    if cd < md
        print(io, ' '^cd, "<expr:call>")

        if cd + 1 < md
            println(io)

            for x in call.args
                tostring(io, x, cd + 1, md)
                if x != last(call.args)
                    println(io)
                end
            end
        end
    end
end

end
