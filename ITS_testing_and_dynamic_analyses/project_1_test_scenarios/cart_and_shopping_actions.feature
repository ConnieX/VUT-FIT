Feature: Cart and Shopping Actions

  Scenario: Add Product to cart
    Given a web browser is at OpenCart "item" page
    And the user is logged in
    When the user fills "parameters"
    And the user add "product" to cart
    Then there is "product" with wanted parameters in cart

  Scenario Outline: Go to Cart
    Given a web browser is at OpenCart home page
    When the user clicks on <button>
    Then page of user's cart is displayed

    Examples:
      | button        |
      | Shopping Cart |
      | View Cart     |

  Scenario: Change count of pieces of product
    Given a web browser is at OpenCart cart page
    And there is "product" in cart
    When the user set Quantity to 3
    Then there are 3 pieces of "product" in cart

  Scenario: Remove Product from Cart
    Given a web browser is at OpenCart cart page
    And one "item" is in a cart
    When the user remove "item" from the cart
    Then there is no item in the cart

  Scenario: Want to Check Out
    Given a web browser is at OpenCart home page
    And in cart is "item"
    When the user clicks on "Check Out"
    Then check out page is displayed

  Scenario: Check Out
    Given a web browser is at OpenCart Check Out page
    When the user fills in data
    Then order is processed

  Scenario: Return Product
    Given the user is logged in OpenCart
    And there is some delivered order
    And a web browser is at OpenCart Order #1 page
    When the user return "product"
    And the user fills data
    Then return request on "product" is send
