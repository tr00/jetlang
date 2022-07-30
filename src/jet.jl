# data structures:

include("ast/ast.h.jl")
# include("cst/cst.h.jl")

# transformations:

include("pcc/pcc.jl")
include("ins/ins.jl")
include("qbe/qbe.jl")

src = "some string"

cst = pcc(src)

ast = ins(cst)

res = qbe(ast)



