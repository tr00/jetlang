include("../ast/ast.h.jl")

using .AST

function execute(atom :: jet_atom_t)
    atom
end

function execute(call :: jet_call_t)
    @assert length(call.args) == 3

    opc, op1, op2 = call.args
        
    op1 = execute(op1)
    op2 = execute(op2)

    if opc === jet_sym_t("add")

        if isa(op1, jet_int_t) && isa(op2, jet_int_t)
            return jet_int_t(op1.val + op2.val)
        end

    elseif opc === jet_sym_t("sub")

        if isa(op1, jet_int_t) && isa(op2, jet_int_t)
            return jet_int_t(op1.val - op2.val)
        end

    elseif opc === jet_sym_t("mul")

        if isa(op1, jet_int_t) && isa(op2, jet_int_t)
            return jet_int_t(op1.val * op2.val)
        end

    elseif opc === jet_sym_t("div")

        if isa(op1, jet_int_t) && isa(op2, jet_int_t)
            return jet_int_t(op1.val ÷ op2.val)
        end

    end

    @error "unimplemented"
end
