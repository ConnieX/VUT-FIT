from selenium import webdriver
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities
from selenium.webdriver import Chrome


def before_all(context):
     context.browser = webdriver.Remote(
       command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',
       desired_capabilities=DesiredCapabilities.CHROME)



def after_all(context):
    context.browser.quit()
