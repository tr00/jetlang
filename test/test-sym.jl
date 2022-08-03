using Test

include("../src/sym/sym.h.jl")

@testset "sym.h.jl" verbose=true begin

    @testset "Csymbol(::String)" begin

        @test Csymbol("a") === Csymbol("a")
        @test Csymbol("b") !== Csymbol("a")
        @test Csymbol("c") !== Csymbol("cd")

    end

    @testset "ncodeunits(::Csymbol) + length(::Csymbol)" begin

        s1, s2 = Csymbol(""), Csymbol("abc")

        @test 0 == ncodeunits(s1)
        @test 3 == ncodeunits(s2)

        @inferred Int ncodeunits(s1)
        @inferred Int ncodeunits(s2)

        @test 0 == length(s1)
        @test 3 == length(s2)

        @inferred Int length(s1)
        @inferred Int length(s2)

    end

    @testset "checkbounds(::Type{Bool}, ::Csymbol, ::Int)" begin

        sym = Csymbol("xyz")

        @test checkbounds(Bool, sym, 0) == false
        @test checkbounds(Bool, sym, 1) == true
        @test checkbounds(Bool, sym, 2) == true
        @test checkbounds(Bool, sym, 3) == true
        @test checkbounds(Bool, sym, 4) == false

        @test checkbounds(sym, 1) === nothing
        @test checkbounds(sym, 2) === nothing
        @test checkbounds(sym, 3) === nothing

        @test_throws BoundsError checkbounds(sym, 0)
        @test_throws BoundsError checkbounds(sym, 4)

    end

    @testset "codeunit(::Csymbol, ::Int) + getindex(::Csymbol, ::Int)" begin

        sym = Csymbol("xyz")

        @test codeunit(sym, 1) == 'x'
        @test codeunit(sym, 2) == 'y'
        @test codeunit(sym, 3) == 'z'

        @inferred Char codeunit(sym, 1)
        @inferred Char codeunit(sym, 2)
        @inferred Char codeunit(sym, 3)

        @test sym[1] == 'x'
        @test sym[2] == 'y'
        @test sym[3] == 'z'

        @inferred Char getindex(sym, 1)
        @inferred Char getindex(sym, 2)
        @inferred Char getindex(sym, 3)

    end

    @testset "string(::Csymbol)" begin

        sym = Csymbol("xyz")

        @test string(sym) == "xyz"

    end

    @testset "iterate(::Csymbol, ::Int)" begin

        sym = Csymbol("xyz")
        arr = ['x', 'y', 'z']

        @test collect(sym) == arr

    end

end