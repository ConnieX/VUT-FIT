from selenium.webdriver.common.by import By
from behave import *
import time


# GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN
# GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN GIVEN
@given('a web browser is at OpenCart home page')
def home_page(context):
    context.browser.get('http://mat.fit.vutbr.cz:8091/index.php?route=common/home')


@given('there is product in cart')
def add_to_cart(context):
    context.browser.get('http://mat.fit.vutbr.cz:8091/index.php?route=common/home')
    item = context.browser.find_element(By.XPATH, "//div[@id='content']/div[2]/div/div/div[3]/button")
    context.id = item.get_attribute('onclick').split('\'')[1]
    context.browser.execute_script(item.get_attribute('onclick'))


@given('a web browser is at OpenCart cart page')
def load_cart_page(context):
    context.browser.get("http://mat.fit.vutbr.cz:8091/index.php?route=checkout/cart")


@given('one item is in a cart')
def add_random_item_to_cart(context):
    context.browser.get('http://mat.fit.vutbr.cz:8091/index.php?route=common/home')
    item = context.browser.find_element(By.XPATH, "//div[@id='content']/div[2]/div/div/div[3]/button")
    context.browser.execute_script(item.get_attribute('onclick'))


@given('a web browser is at OpenCart Check Out page')
def check_out_page(context):
    context.browser.get('http://mat.fit.vutbr.cz:8091/index.php?route=checkout/checkout')


# WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN
# WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN WHEN
@when('the user add first "product" to cart')
def add_to_cart(context):
    item = context.browser.find_element(By.XPATH, "//div[@id='content']/div[2]/div/div/div[3]/button")
    context.id = item.get_attribute('onclick').split('\'')[1]
    context.browser.execute_script(item.get_attribute('onclick'))


@when('the user clicks on ShoppingCart')
def go_to_empty_cart(context):
    context.browser.find_element(By.XPATH,
                                 "//a[@href='http://mat.fit.vutbr.cz:8091/index.php?route=checkout/cart']").click()


@when('the user clicks on ViewCart')
def go_to_cart(context):
    context.browser.find_element(By.XPATH, "//div[@id='cart']/button/i").click()
    context.browser.find_element(By.XPATH, "//div[@id='cart']/ul/li[2]/div/p/a/strong").click()


@when('the user set Quantity to 9')
def change_quantity(context):
    context.browser.find_element(By.XPATH, "//td[4]/div/input").click()
    context.browser.find_element(By.XPATH, "//td[4]/div/input").send_keys('\b')
    context.browser.find_element(By.XPATH, "//td[4]/div/input").send_keys("9")
    context.browser.find_element(By.XPATH, "//button[@type='submit']").click()


@when('the user remove item from the cart')
def remove_item(context):
    context.remove_element = context.browser.find_element(By.XPATH, "//div[@id='content']/form/div/table/tbody/tr/td[2]/a").text
    context.browser.find_element(By.XPATH, "//button[2]/i").click()


@when('the user clicks on Check Out')
def check_out_click(context):
    time.sleep(0.5)
    context.browser.find_element(By.XPATH, "//div[@id='content']/div[3]/div[2]/a").click()


@when('the user fills in data')
def fill_data(context):
    time.sleep(0.5)
    context.browser.find_element(By.XPATH, "//div[@id='collapse-checkout-option']/div/div/div/div[2]/label").click()
    context.browser.find_element(By.XPATH, "//input[@id='button-account']").click()
    time.sleep(0.5)
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-firstname']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-firstname']").send_keys("Eddie")
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-lastname']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-lastname']").send_keys("Eagle")
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-email']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-email']").send_keys("admin@admin.cz")
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-telephone']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-telephone']").send_keys("111232333")
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-address-1']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-address-1']").send_keys("Blah")
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-city']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-city']").send_keys("Brno")
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-postcode']").click()
    context.browser.find_element(By.XPATH, "//input[@id='input-payment-postcode']").send_keys("65000")
    context.browser.find_element(By.ID, "input-payment-zone").click()
    time.sleep(0.5)
    context.browser.find_element(By.ID, "input-payment-zone").send_keys("A")
    time.sleep(0.5)
    context.browser.find_element(By.ID, "input-payment-zone").send_keys('\t')
    context.browser.find_element(By.XPATH, "//input[@id='button-guest']").click()
    time.sleep(0.5)
    context.browser.find_element(By.XPATH, "//input[@name='agree']").click()
    context.browser.find_element(By.XPATH, "//input[@id='button-payment-method']").click()
    time.sleep(0.5)
    context.browser.find_element(By.XPATH, "//input[@id='button-confirm']").click()
    time.sleep(0.5)


# THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN
# THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN THEN
@then('there is "product" with wanted parameters in cart')
def control_cart(context):
    context.browser.get("http://mat.fit.vutbr.cz:8091/index.php?route=checkout/cart")
    element = context.browser.find_element(By.XPATH, "//div[@id='content']/form/div/table/tbody/tr/td[2]/a")
    id = element.get_attribute('href')[-2:]
    assert (id == context.id)


@then('page of users cart is displayed')
def control_cart_page(context):
    assert (context.browser.find_element(By.XPATH, "//div[@id='content']/div[3]/div/a"))


@then('there are 9 pieces of product in cart')
def check_pieces(context):
    element = context.browser.find_element(By.XPATH, "//div[@id='content']/form/div/table/tbody/tr")
    element = element.text.split("$")
    total = int(element[2].split(".")[0].replace(",", "", 1))
    one = int(element[1].split(".")[0].replace(",", "", 1))
    pieces = int(total / one)
    assert (pieces == 9)


@then('there is no item in the cart')
def control_no_item(context):
    some_item = True
    time.sleep(2)
    try:
        context.browser.find_element(By.XPATH, "//div[@id='content']/form/div/table/tbody/tr/td[2]/a")
    except:
        some_item = False
    if some_item:
        assert (context.browser.find_element(By.XPATH, "//div[@id='content']/form/div/table/tbody/tr/td[2]/a").text != context.remove_element)


@then('check out page is displayed')
def check_out_page(context):
    element = context.browser.find_element(By.ID, "content").text
    first_word = element.split("\n")[0]
    assert (first_word == "Checkout")


@then('order is processed')
def checked_out(context):
    time.sleep(0.5)
    element = context.browser.find_element(By.ID, "content").text
    first_word = element.split("\n")[0]
    assert (first_word == "Your order has been placed!")
