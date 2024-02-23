Feature: Movement on the Page

  Scenario: Get on the Page of Item
    Given a web browser is at OpenCart home page
    When the user click at first product link
    Then the page for chosen product link is displayed

  Scenario: Get on the Page of Category Items
    Given a web browser is at OpenCart home page
    When the user click on category link
    Then the page for chosen category link is displayed

  Scenario: Add Product to Wish List
    Given a web browser is at OpenCart "product" page
    And the user is logged in
    When the user clicks on "Add to wish list"
    Then "product" is in wish list

  Scenario: Remove Product from Wish List
    Given the user is logged in OpenCart
    And s web browser is at OpenCart wishlist page
    And in wish list is "product"
    When the user clicks on "Remove" button for this "product"
    Then "product" is not in wish list

  Scenario: Add Product to Comparison:
    Given a web browser is at "product" page
    When the user clicks on "Add to comparison"
    Then "product" is in product comparison

  Scenario: Remove Product from Comparison
    Given a web browser is at OpenCart product comparison page
    And there is "product" in product comparison
    When the user click on "Remove" button for this "product"
    Then "product" is not in product comparison

  Scenario: Go to Home Page
    Given a web browser is at OpenCart cart page
    When the user click at "Your Store" button
    Then OpenCart home page is displayed

  Scenario Outline: Change Currency to Pound
    Given a web browser is at OpenCart home page
    When the user changes currency to "<curr>"
    Then prices on web are displayed in "<currs>"

    Examples: Currency
      | curr           | currs           |
      | Euro           | Euros           |
      | US Dollar      | US Dollars      |
      | Pound Sterling | Pounds Sterling |