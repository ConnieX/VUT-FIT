Feature: Account Actions

  Scenario: Create New Account
    Given a web browser is at OpenCart registration page
    When the user fills in form
    Then account is created
    And the user is logged in

    Scenario: Log Out
    Given the user is logged in OpenCart
    When the user click on "Log out"
    Then the user is not logged in

  Scenario: Log In
    Given a web browser is at OpenCart login page
    When the user fill in fills in correct e-mail and password
    Then the user is logged in

  Scenario: Log Out
    Given the user is logged in OpenCart
    When the user click on "Log out"
    Then the user is not logged in