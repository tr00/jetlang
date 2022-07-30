include("../ast/ast.h.jl")

using .AST

using AbstractTrees
using Plots, GraphRecipes

AbstractTrees.nodevalue(call :: jet_call_t) = first(call.args)
AbstractTrees.children(call :: jet_call_t) = @view call.args[2:end]




