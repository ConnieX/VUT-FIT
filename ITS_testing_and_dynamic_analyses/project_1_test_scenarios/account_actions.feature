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

  Scenario: Change Password
    Given the user is logged in OpenCart
    And a web browser is at OpenCart password page
    When user fills in "new password"
    Then password is changed to "new password"

  Scenario: Change Account Information:
    Given the user is logged in OpenCart
    And a web browser is at OpenCart My Account Information page
    When user fills in "new first name" in First name
    Then first name is changed to "new first name"

  Scenario: Newsletter Sign Up
    Given the user is logged in OpenCart
    And a web browser is at OpenCart newsletter page
    And subscribe is set at "No"
    When the user set subscribe at "Yes"
    Then newsletter subscription is updated to "Yes"

  Scenario: Newsletter Sign Out
    Given the user is logged in OpenCart
    And a web browser is at OpenCart newsletter page
    And subscribe is set at "Yes"
    When the user set subscribe at "No"
    Then newsletter subscription is updated to "No"

  Scenario: Edit Address Book:
    Given the user is logged in OpenCart
    And a web browser is at OpenCart edit address page
    When the user fills in "data"
    Then address contains new "data"

  Scenario: Add New Address:
    Given the user is logged in OpenCart
    And a web browser as at OpenCart add address page
    When the user fills in "data"
    Then new address with "data" is in Address Book