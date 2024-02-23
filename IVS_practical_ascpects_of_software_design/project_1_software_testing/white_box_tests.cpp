//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     Monika Rosinska <xrosin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Monika Rosinska
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech
//    matic.
//============================================================================//

class Matrices : public ::testing::Test
{
  protected:
    Matrix matrix;
};

TEST_F(Matrices, EmptyMatrix)
{
    Matrix m;
    EXPECT_TRUE(m.get(0, 0) == 0);
    EXPECT_ANY_THROW(m.get(1, 1));
    EXPECT_ANY_THROW(m.get(-1, 10));
    EXPECT_ANY_THROW(m.get(45, 20));
}

TEST_F(Matrices, SizedMatrix)
{
    Matrix m(2, 5);
    EXPECT_TRUE(m.get(0, 0) == 0);
    EXPECT_TRUE(m.get(1, 4) == 0);
    EXPECT_ANY_THROW(m.get(2, 5));
    EXPECT_ANY_THROW(m.get(-1, 10));

    Matrix n(1, 1);
    EXPECT_TRUE(n.get(0, 0) == 0);
    EXPECT_ANY_THROW(n.get(1, 1) == 0);
    EXPECT_ANY_THROW(n.get(2, 5));
    EXPECT_ANY_THROW(n.get(-1, 10));

    Matrix o(20, 1);
    EXPECT_TRUE(o.get(0, 0) == 0);
    EXPECT_TRUE(o.get(19, 0) == 0);
    EXPECT_ANY_THROW(o.get(21, 1));
    EXPECT_ANY_THROW(o.get(1, 2));

    EXPECT_ANY_THROW(Matrix p(0, 0));
    EXPECT_ANY_THROW(Matrix q(-2, -6));
}

TEST_F(Matrices, ChangigValue)
{
    Matrix m(2, 5);
    EXPECT_TRUE(m.set(0, 3, 2));
    EXPECT_TRUE(m.get(0, 3) == 2);
    EXPECT_TRUE(m.get(0, 0) == 0);
    EXPECT_TRUE(m.get(0, 2) == 0);
    EXPECT_TRUE(m.get(1, 3) == 0);

    EXPECT_FALSE(m.set(2, 5, 6));
    EXPECT_FALSE(m.set(-1, -1, 5));

    Matrix n(3, 18);
    EXPECT_TRUE(n.set(2, 6, 7));
    EXPECT_TRUE(n.get(2, 6) == 7);
    EXPECT_FALSE(n.set(2, 19, 5));
    EXPECT_FALSE(n.set(4, 2, 5));
}

TEST_F(Matrices, SetFromArray)
{
    Matrix m(4, 4);
    EXPECT_TRUE(m.set(std::vector<std::vector<double>>{
        {1, 2, -9, 26},
        {3, 5, 85, 6.04},
        {45, 65, 2, 6},
        {0, -6, -5.1, 2},
    }));

    EXPECT_FALSE(m.set(std::vector<std::vector<double>>{
        {1, 2, -9, 26},
        {3, 5, 85, 6.04},
        {45, 65, 2, 6},
    }));

    EXPECT_FALSE(m.set(std::vector<std::vector<double>>{
        {1, 2, -9, 26},
        {3, 5, 85, 6.04},
        {45, 65, 2, 6},
        {0, -6, -5.1, 2},
        {0, 1, 2, 3},
    }));

    EXPECT_FALSE(m.set(std::vector<std::vector<double>>{
        {1, 2, -9},
        {3, 5, 85, 6.04},
        {45, 65, 2, 6},
        {0, -6, -5.1, 2},
    }));

    EXPECT_FALSE(m.set(std::vector<std::vector<double>>{
        {1, 2, -9, 26, 20},
        {3, 5, 85, 6.04},
        {45, 65, 2, 6},
        {0, -6, -5.1, 2},
    }));

    Matrix n(3, 8);
    EXPECT_TRUE(n.set(std::vector<std::vector<double>>{
        {1, 2, 3, 4, 5, 6, 7, 8},
        {1, 2, 4, 6, 8, 10, 12, 14},
        {1, 0, 1, 0, 1, 0, 1, 0},
    }));
    EXPECT_FALSE(n.set(std::vector<std::vector<double>>{
        {1, 2, -9, 26},
        {3, 5, 85, 6.04},
        {45, 65, 2, 6},
        {0, -6, -5.1, 2},
    }));
}

TEST_F(Matrices, Equality)
{
    Matrix m(2, 2);
    Matrix n(2, 2);
    Matrix o(3, 2);
    Matrix p(5, 13);
    Matrix q(5, 13);

    EXPECT_TRUE(m == n);
    EXPECT_ANY_THROW(m == o);
    EXPECT_ANY_THROW(m == p);
    EXPECT_TRUE(p == q);

    m.set(std::vector<std::vector<double>>{
        {1, 2},
        {3, 4},
    });

    EXPECT_FALSE(m == n);

    n.set(std::vector<std::vector<double>>{
        {2, 1},
        {4, 3},
    });

    EXPECT_FALSE(m == n);

    n.set(std::vector<std::vector<double>>{
        {1, 2},
        {3, 4},
    });

    EXPECT_TRUE(m == n);

    o.set(std::vector<std::vector<double>>{
        {1, 2, 5},
        {3, 4, 6},
    });

    EXPECT_ANY_THROW(m == o);

    p.set(std::vector<std::vector<double>>{
        {1, 2, 13, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
        {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
        {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    });

    q.set(std::vector<std::vector<double>>{
        {1, 2, 13, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
        {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
        {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    });

    EXPECT_TRUE(p == q);

    q.set(std::vector<std::vector<double>>{
        {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
        {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 2, 13, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
    });

    EXPECT_FALSE(p == q);
}

TEST_F(Matrices, Addition)
{
    Matrix m(2, 2);
    Matrix n(2, 2);
    Matrix o(3, 2);
    Matrix control(2, 2);

    EXPECT_TRUE((m + n) == control);
    EXPECT_ANY_THROW(m + o);
    EXPECT_ANY_THROW(n + o);

    m.set(std::vector<std::vector<double>>{
        {1, 2},
        {3, 4},
    });
    control.set(std::vector<std::vector<double>>{
        {1, 2},
        {3, 4},
    });

    EXPECT_TRUE((m + n) == control);

    n.set(std::vector<std::vector<double>>{
        {4, 3},
        {2, 1},
    });
    control.set(std::vector<std::vector<double>>{
        {5, 5},
        {5, 5},
    });

    EXPECT_TRUE((m + n) == control);

    n.set(std::vector<std::vector<double>>{
        {-4, -3},
        {-2, -1},
    });
    control.set(std::vector<std::vector<double>>{
        {-3, -1},
        {1, 3},
    });

    EXPECT_TRUE((m + n) == control);
}

TEST_F(Matrices, Multiplication)
{
    Matrix m(2, 2);
    Matrix n(2, 2);
    Matrix o(3, 2);
    Matrix control(2, 2);
    Matrix control2(3, 2);

    EXPECT_TRUE(m * n == control);
    EXPECT_TRUE(o * m == control2);
    EXPECT_ANY_THROW(m * o);
    EXPECT_TRUE(o * n == control2);

    m.set(std::vector<std::vector<double>>{
        {1, 2},
        {3, 4},
    });

    EXPECT_TRUE(m * n == control);

    n.set(std::vector<std::vector<double>>{
        {4, 3},
        {2, 1},
    });
    control.set(std::vector<std::vector<double>>{
        {8, 5},
        {20, 13},
    });

    EXPECT_TRUE(m * n == control);

    n.set(std::vector<std::vector<double>>{
        {-4, -3},
        {-2, 1},
    });
    control.set(std::vector<std::vector<double>>{
        {-8, -1},
        {-20, -5},
    });

    EXPECT_TRUE(m * n == control);
}

TEST_F(Matrices, ScalarMultiplication)
{
    Matrix m(2, 2);
    Matrix n(3, 2);
    Matrix control(2, 2);
    Matrix control2(3, 2);

    EXPECT_TRUE(m * 2.0 == control);
    EXPECT_TRUE(n * 2.0 == control2);

    EXPECT_TRUE(m * 0 == control);
    EXPECT_TRUE(n * 0 == control2);

    m.set(std::vector<std::vector<double>>{
        {1, 2},
        {3, 4},
    });
    control.set(std::vector<std::vector<double>>{
        {2, 4},
        {6, 8},
    });

    n.set(std::vector<std::vector<double>>{
        {4, 3},
        {2, 1},
    });
    control2.set(std::vector<std::vector<double>>{
        {8, 6},
        {4, 2},
    });

    EXPECT_TRUE(m * 2.0 == control);
    EXPECT_TRUE(n * 2.0 == control2);

    control.set(std::vector<std::vector<double>>{
        {1.5, 3},
        {4.5, 6},
    });
    control2.set(std::vector<std::vector<double>>{
        {-6, -4.5},
        {-3, -1.5},
    });

    EXPECT_TRUE(m * 1.5 == control);
    EXPECT_TRUE(n * -1.5 == control2);
}

TEST_F(Matrices, SolveEquation)
{
    Matrix m(2, 2);
    Matrix n(3, 3);
    Matrix o(5, 4);
    Matrix p(4, 5);
    Matrix q(2, 2);
    Matrix r;
    Matrix s(6, 6);
    Matrix t(1, 1);

    m.set(std::vector<std::vector<double>>{
        {1, 2},
        {-2, 1},
    });
    n.set(std::vector<std::vector<double>>{
        {4, 3, -3},
        {1, 1, -1},
        {2, 2, -1},
    });
    q.set(std::vector<std::vector<double>>{
        {1, 1},
        {2, 2},
    });
    s.set(std::vector<std::vector<double>>{
        {1, 1, 1, 1, 1, 1},
        {2, 1, 2, 1, 2, 1},
        {1, 2, 3, 4, 5, 6},
        {2, 3, 8, 7, 1, 5},
        {1, 5, 3, 6, 2, 1},
        {7, 8, 1, 2, 0, 0},
    });
    t.set(std::vector<std::vector<double>>{
        {1}});

    EXPECT_TRUE(m.solveEquation(std::vector<double>{3, -1}) == (std::vector<double>{1, 1}));
    EXPECT_TRUE(n.solveEquation(std::vector<double>{0, 0, 1}) == (std::vector<double>{0, 1, 1}));
    EXPECT_ANY_THROW(o.solveEquation(std::vector<double>{0, 0, 0, 0}));
    EXPECT_ANY_THROW(p.solveEquation(std::vector<double>{0, 0, 0, 0, 0}));
    EXPECT_ANY_THROW(m.solveEquation(std::vector<double>{3, -1, 2}));
    EXPECT_ANY_THROW(n.solveEquation(std::vector<double>{3, -1}));
    EXPECT_ANY_THROW(q.solveEquation(std::vector<double>{1, 2}));
    EXPECT_ANY_THROW(r.solveEquation(std::vector<double>{1}));
    EXPECT_TRUE(s.solveEquation(std::vector<double>{21, 30, 91, 95, 60, 34}) == (std::vector<double>{1, 2, 3, 4, 5, 6}));
    EXPECT_TRUE(t.solveEquation(std::vector<double>{1}) == (std::vector<double>{1}));
}

/*** Konec souboru white_box_tests.cpp ***/