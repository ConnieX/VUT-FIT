from selenium.webdriver.common.by import By
from behave import *
import time


# GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN
# GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN
@given('a web browser is at OpenCart registration page')
def reg_page(context):
    context.browser.get('http://mat.fit.vutbr.cz:8091/index.php?route=account/register')
    time.sleep(2)


@given('the user is logged in OpenCart')
def logged_in(context):
    context.browser.get('http://mat.fit.vutbr.cz:8091/index.php?route=common/home')

    try:
        context.browser.find_element(By.XPATH, "//a[contains(text(),'Logout')]")
    except:
        context.browser.get('http://mat.fit.vutbr.cz:8091/index.php?route=account/login')
        context.browser.find_element(By.XPATH, "//input[@id='input-email']").click()
        context.browser.find_element(By.XPATH, "//input[@id='input-email']").send_keys("admin42@admin.cz")
        context.browser.find_element(By.XPATH, "//input[@id='input-password']").click()
        context.browser.find_element(By.XPATH, "//input[@id='input-password']").send_keys("admin")
        context.browser.find_element(By.XPATH, "//input[@value='Login']").click()


@given('a web browser is at OpenCart login page')
def login_page(context):
    context.browser.get('http://mat.fit.vutbr.cz:8091/index.php?route=account/login')


# WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN
# WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN
@when('the user fills in form')
def fill_form(context):
    context.browser.find_element(By.XPATH, "//input[@id='input-firstname']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-firstname']").send_keys("Eddie")
    context.browser.find_element(By.XPATH, "//input[@id='input-lastname']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-lastname']").send_keys("Eagle")
    context.browser.find_element(By.XPATH, "//input[@id='input-email']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-email']").send_keys(
        "admin42@admin.cz")
    context.browser.find_element(By.XPATH, "//input[@id='input-telephone']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-telephone']").send_keys("111232333")
    context.browser.find_element(By.XPATH, "//input[@id='input-address-1']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-address-1']").send_keys("Blah")
    context.browser.find_element(By.XPATH, "//input[@id='input-city']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-city']").send_keys("Brno")
    context.browser.find_element(By.XPATH, "//input[@id='input-postcode']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-postcode']").send_keys("65000")
    context.browser.find_element(By.ID, "input-zone").click()
    time.sleep(0.5)
    context.browser.find_element(By.ID, "input-zone").send_keys("A")
    time.sleep(0.5)
    context.browser.find_element(By.ID, "input-zone").send_keys('\t')
    context.browser.find_element(By.XPATH, "//input[@id='input-password']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-password']").send_keys("admin")
    context.browser.find_element(By.XPATH, "//input[@id='input-confirm']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-confirm']").send_keys("admin")
    context.browser.find_element(By.XPATH, "//input[@name='agree']").click()
    context.browser.find_element(By.XPATH, "//input[@value='Continue']").click()
    time.sleep(0.5)


@when('the user click on "Log out"')
def log_out(context):
    item = context.browser.find_element(By.XPATH, "//div[@id='top-links']/ul/li[2]/ul/li[5]/a")
    context.browser.get(item.get_attribute("href"))
    context.browser.find_element(By.XPATH, "//a[contains(text(),'Continue')]").click()


@when('the user fill in fills in correct e-mail and password')
def log_in(context):
    context.browser.find_element(By.XPATH, "//input[@id='input-email']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-email']").send_keys("admin42@admin.cz")
    context.browser.find_element(By.XPATH, "//input[@id='input-password']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-password']").send_keys("admin")
    context.browser.find_element(By.XPATH, "//input[@value='Login']").click()


# THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN
# THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN
@then('account is created')
def acc_created(context):
    time.sleep(0.5)
    element = context.browser.find_element(By.ID, "content").text
    first_word = element.split("\n")[0]
    assert (first_word == "Your Account Has Been Created!")
    context.browser.find_element(By.XPATH, "//a[contains(.,'Continue')]").click()


@then('the user is logged in')
def logged_in(context):
    time.sleep(0.5)
    try:
        context.browser.find_element(By.XPATH, "//a[contains(.,'Logout')]")
    except:
        logged_in = False
        assert (logged_in is True)


@then('the user is not logged in')
def not_logged(context):
    try:
        context.browser.find_element(By.XPATH, "//a[contains(text(),'Login')]")
    except:
        logged_in = True
        assert (logged_in is False)
