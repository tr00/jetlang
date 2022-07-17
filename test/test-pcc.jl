using Test

include("src/ast.jl")

@testset "pcc valid inputs" begin

    a = jet_sym_t("a")
    b = jet_sym_t("b")
    c = jet_sym_t("c")

    @test parse_string("a")         == a
    @test parse_string("(a b)")     == jet_expr_t(a, b)
    @test parse_string("(a b c)")   == jet_expr_t(a, b, c)
    @test parse_string("(a (b c))") == jet_expr_t(a, jet_expr_t(b, c))
    @test parse_string("((a b) c)") == jet_expr_t(jet_expr_t(a, b), c)

end

@testset "pcc invalid inputs" begin
    
    @test !false

end
