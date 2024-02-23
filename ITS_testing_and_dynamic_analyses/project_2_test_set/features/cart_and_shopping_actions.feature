Feature: Cart and Shopping Actions


  Scenario: Add Product to cart from home page
    Given a web browser is at OpenCart home page
    When the user add first "product" to cart
    Then there is "product" with wanted parameters in cart

  Scenario: Go to Empty Cart
    Given a web browser is at OpenCart home page
    When the user clicks on ShoppingCart
    Then page of users cart is displayed

  Scenario: Go to Cart
    Given there is product in cart
    And a web browser is at OpenCart home page
    When the user clicks on ViewCart
    Then page of users cart is displayed

  Scenario: Change count of pieces of product
    Given there is product in cart
    And a web browser is at OpenCart cart page
    When the user set Quantity to 9
    Then there are 9 pieces of product in cart

  Scenario: Remove Product from Cart
    Given there is product in cart
    And a web browser is at OpenCart cart page
    When the user remove item from the cart
    Then there is no item in the cart

  Scenario: Want to Check Out
   Given there is product in cart
   And a web browser is at OpenCart cart page
   When the user clicks on Check Out
   Then check out page is displayed

  Scenario: Check Out
    Given there is product in cart
    And a web browser is at OpenCart Check Out page
    When the user fills in data
    Then order is processed
