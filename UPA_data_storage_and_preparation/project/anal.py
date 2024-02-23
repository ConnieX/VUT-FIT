import os

import pymongo
from pandas.core.common import SettingWithCopyWarning
from pymongo import MongoClient
import csv
import json

import warnings
warnings.simplefilter(action='ignore', category=SettingWithCopyWarning)
from pandas import json_normalize
from bson.json_util import dumps
import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns
import numpy as np
from scipy import stats
import unicodedata

# Class containing region, count of inhabitants (children and adults) and
# list of dictionaries with key "OborPece" and value representing the number of children and adults in the region
class Element:
    def __init__(self, okres):
        self.okres = okres
        self.pocet_obyv_dorast = 0
        self.pocet_obyv_dospeli = 0
        self.vsetci_obyvatelia = 0
        self.poskytovatelia = []
        self.percento_dorast = 0
        self.percento_dospeli = 0

    def append_poskytovatel(self, poskytovatel):
        if not any(poskytovatel in p for p in self.poskytovatelia):
            self.poskytovatelia.append({poskytovatel: 1})
        else:
            for p in self.poskytovatelia:
                for key, value in p.items():
                    if key == poskytovatel:
                        p[key] = p[key] + 1

    def print_element(self):
        return "'Okres': '{}', 'Počet obyvateľov dorast': {}, 'Počet obyvateľov dospelí': {}, 'Počet všetkých obyvateľov': {}, " \
               "'Percento dorast': {}, 'Percento dospelí': {}, 'Poskytovatelia': {}".format(self.okres,
                                                                                            self.pocet_obyv_dorast,
                                                                                            self.pocet_obyv_dospeli,
                                                                                            self.vsetci_obyvatelia,
                                                                                            self.percento_dorast,
                                                                                            self.percento_dospeli,
                                                                                            self.poskytovatelia)


def set_up_database():
    # Pripojí sa na zhluk, v ktorom je databáza

    cluster = MongoClient(
        "mongodb+srv://new-user:123@cluster0.mc4yx.mongodb.net/zdravotnictvivcr?retryWrites=true&w=majority&ssl=true&ssl_cert_reqs=CERT_NONE"
    )

    # Pripojí sa k databázi
    db = cluster["zdravotnictvivcr"]

    # Pripojí sa ku kolekciám dát, ktoré reprezentujú CSVčka pre:
    # "Národní registr poskytovatelů zdravotních služeb" - NarReg
    # "Data Českého statistického úřadu o obyvatelstvu ČR" - CZStat
    coll_query_a = db["DotazA"]
    coll_query_b_provider = db["DotazB_Poskytovatelia"]
    coll_query_b_inhabitants = db["DotazB_Obyvatelia"]
    coll_query_custom = db["DotazCustom_Lekaren"]
    coll_query_custom_providers = db["DotazCustom_Poskytovatelia"]
    coll_query_custom_inhabitants = db["DotazCustom_Obyvatelia"]

    return coll_query_a, coll_query_b_inhabitants, coll_query_b_provider, coll_query_custom_inhabitants, \
           coll_query_custom_providers, coll_query_custom


def create_csv(filename, ldicts):
    f = open(filename, 'a')
    for df in ldicts:
        df.to_csv(f)
    f.close()


# function for saving picture (graphs) to folder "graphs"
# if folder is not created, function will create it
def save_file(filename, picture):
    if not os.path.exists('graphs'):
        os.mkdir('graphs')

    # remove diacritics and replace spaces with '_'
    filename = ''.join(c for c in unicodedata.normalize('NFD', filename)
                       if unicodedata.category(c) != 'Mn')
    filename = filename.replace(" ", "_")
    filename = filename.lower()

    picture.savefig(fname='graphs/%s.png' % filename,
                    facecolor=picture.get_facecolor(),
                    bbox_inches="tight")


# prepare providers data for query A (include only providers from Brno and other regions from Jihomoravský kraj)
def prepare_df_a_providers(region, df, providers):
    df = df[['NazevCely', 'Kraj', 'KrajKod', 'Okres', 'OkresKod', 'OborPece']]
    df = df[df['Kraj'].isin(['Jihomoravský kraj'])]
    if region == 'Brno':
        df = df[df['Okres'].isin(['Brno-město'])]
    else:
        df = df[df['Okres'].isin(['Brno-město']) == False]
    df = df[df['OborPece'].isin(providers)]
    return df


# plot function for query A
# plots graph of given city and region
def plot_providers(df_city, df_region, city_name, region_name):
    df_city = df_city[['OborPece', 'NazevCely']]
    city = df_city.groupby('OborPece').count()
    city = city.rename(columns={'NazevCely': 'Pocet'})
    city = city.reset_index()
    # dead = dead.sort_values(by=['dead'], ascending=False)
    city = city.sort_values(by=['Pocet'], ascending=False)

    df_region = df_region[['OborPece', 'NazevCely']]
    region = df_region.groupby('OborPece').count()
    region = region.rename(columns={'NazevCely': 'Pocet'})
    region = region.reset_index()
    # dead = dead.sort_values(by=['dead'], ascending=False)
    region = region.sort_values(by=['Pocet'], ascending=False)

    # set graph appearance
    fig = plt.figure(figsize=(9.8, 11))
    fig.tight_layout()
    fig.subplots_adjust(top=0.94)
    fig.suptitle('Počet poskytovatelů v Brně a Jihomoravském kraji')
    fig.subplots_adjust(hspace=0.8, bottom=0.2)
    fig.text(0.51, 0.003, 'Poskytovatelé', ha='center', va='bottom')

    axs = fig.subplots(2)
    sns.barplot(ax=axs[0],
                x=city.OborPece,
                y=city.Pocet,
                data=city,
                palette="viridis")
    sns.barplot(ax=axs[1],
                x=region.OborPece,
                y=region.Pocet,
                data=region,
                palette="viridis")
    x_titles = [city_name, region_name]

    for i in range(2):
        axs[i].set_ylabel('Počet', size=10)
        axs[i].set_xlabel('')

        axs[i].set_title(x_titles[i])

        axs[i].set_axisbelow(True)
        axs[i].tick_params(axis='x', rotation=90)

    plt.show()
    save_file(city_name + "_" + region_name, fig)


# prepare historical data of providers of given specialization (date of registration as provider and if it's still active)
def prepare_df_a_history(df_a, specialization):
    df_a = df_a[df_a['OborPece'].isin([specialization])]

    df_a = df_a[['OborPece', 'DatumZahajeniCinnosti']]
    df_a = df_a.groupby('DatumZahajeniCinnosti').count().reset_index()
    df_a = df_a.rename(columns={'OborPece': 'Count'})

    # resampling to quarterly values
    df_a = df_a.resample('QS', on='DatumZahajeniCinnosti').agg({'Count': 'sum'}).reset_index()

    # counting values together so that in the column 'Sum' will be sum of providers from all quarters before
    df_a['Sum'] = df_a.Count.cumsum()
    plot_a_history_providers(df_a, specialization)


# plot function for query A
# plots graph of count of providers in history and graph representing count of new providers in history
def plot_a_history_providers(df, specialization):
    fig = plt.figure(figsize=(9.8, 11))
    fig.subplots_adjust(top=0.94)
    fig.suptitle('Počet poskytovatelov v oboru ' + specialization)
    fig.subplots_adjust(hspace=0.8, bottom=0.2)
    fig.text(0.5, 0.1, 'Čas', ha='center', va='bottom')

    axs = fig.subplots(2)
    sns.lineplot(ax=axs[1], data=df, x="DatumZahajeniCinnosti", y="Count")
    sns.lineplot(ax=axs[0], data=df, x="DatumZahajeniCinnosti", y="Sum")
    x_titles = ['Počet aktivních poskytovatelů', 'Počet nových poskytovatelů']

    for i in range(2):
        axs[i].set_ylabel('Počet', size=10)
        axs[i].set_xlabel('')

        axs[i].set_title(x_titles[i])

        axs[i].set_axisbelow(True)
        axs[i].tick_params(axis='x', rotation=90)

    plt.show()
    save_file('historie_' + specialization, fig)


def query_a():
    # convert document into pandas dataframe (via string and JSON object) and save it as DotazA.csv
    document = list_of_collections[0].find({})
    string = dumps(document)
    json_obj = json.loads(string)
    data1 = json_normalize(json_obj)
    data1.to_csv("DotazA.csv")

    df_a = pd.read_csv('DotazA.csv')
    df_a['DatumZahajeniCinnosti'] = pd.to_datetime(
        df_a['DatumZahajeniCinnosti'])

    providers = [
        'všeobecné praktické lékařství', 'zubní lékařství', 'psychiatrie',
        'Dentální hygienistka', 'Fyzioterapeut', 'Zubní technik', 'chirurgie',
        'neurologie', 'urologie', 'pediatrie', 'kardiologie', 'patologie',
        'traumatologie', 'gynekologie a porodnictví', 'Ortoptista'
    ]
    df_a_jhk = prepare_df_a_providers('JHK', df_a, providers)
    df_a_brno = prepare_df_a_providers('Brno', df_a, providers)
    plot_providers(df_a_brno, df_a_jhk, 'Brno', 'Jihomoravský kraj')

    prepare_df_a_history(df_a, 'Dentální hygienistka')
    prepare_df_a_history(df_a, 'všeobecné praktické lékařství')
    prepare_df_a_history(df_a, 'zubní lékařství')
    prepare_df_a_history(df_a, 'pediatrie')
    prepare_df_a_history(df_a, 'psychiatrie')


def transform_dict_to_dataframe(dict_to_transform, column_names):
    df = pd.DataFrame.from_dict(dict_to_transform,
                                orient='index').reset_index()
    df.columns = column_names
    return df


def prepare_dict_b():
    # Vytiahnem si dáta z kolekcie DotazB_Obyvatelia, ktoré sortnem podľa kraju
    sort = {"$sort": {"vuzemi_txt": 1}}
    # Matchne regex pri vek_txt na obyvateľo 20+
    match = {"$match": {"vek_txt": {"$regex": "^[2-9][05]"}}}
    pipeline = [sort, match]
    results = list_of_collections[1].aggregate(pipeline)
    # Dict, kde je key kraj a value súčet obyvateľov v kraji od veku 20+
    regions_inhabitants = {}
    for result in results:
        # Ak nie je kraj ako key v dicte, pridaj ho tam a daj mu hodnotu, ktorá je uložená v prvom objekte
        if result["vuzemi_txt"] not in regions_inhabitants:
            regions_inhabitants[result["vuzemi_txt"]] = 0 # Set the value to 0 when its created - it will get updated on the next line
        # The region is now a key in the dictionary, so we can add the value of the result to the value of the key
        regions_inhabitants[result["vuzemi_txt"]] += int(result["hodnota"])

    # Vytiahnem si dáta z kolekcie DotazB_Poskytovatelia, ktoré sortnem podľa kraju
    sort = {"$sort": {"Kraj": 1}}
    pipeline = [sort]
    results = list_of_collections[2].aggregate(pipeline)

    # Dict, kde je key kraj a value počet poskytovateľov v kraji
    regions_providers = {}
    for result in results:
        # Ak nie je kraj ako key v dicte, pridaj ho tam a vlož tam 1 (prvý postytovateľ v kraji)
        if result["Kraj"] not in regions_providers:
            regions_providers[result["Kraj"]] = 0 # we can set the value to 0 when its created - it will get updated on the next line
        regions_providers[result["Kraj"]] += 1 # and the region is now a key in the dictionary, so we can add 1 to the value of the key

    # X = X prienik Y
    regions_inhabitants = {
        x: regions_inhabitants[x]
        for x in regions_inhabitants if x in regions_providers
    }
    # Y = Y prienik X
    regions_providers = {
        x: regions_providers[x]
        for x in regions_providers if x in regions_inhabitants
    }

    # Dict, kde je key kraj a value počet poskytovateľov/počet obyvateľov v kraji
    regions_rate = {}
    for region_provider in regions_providers:
        for region_inhabitant in regions_inhabitants:
            if region_inhabitant == region_provider:
                regions_rate[region_provider] = regions_inhabitants[region_inhabitant] / regions_providers[region_provider]
    return regions_inhabitants, regions_providers, regions_rate


def plot_inhabitants_providers_rate(df_b, df_b_rate, graph_name):
    fig = plt.figure(figsize=(10, 5))
    fig.tight_layout()
    fig.subplots_adjust(bottom=0.4)  # or whatever

    ax1 = fig.add_subplot(111)
    ax2 = ax1.twinx()
    # sns.barplot(x='Kraj', y='Sum_Providers', data=df_B, color='blue', ax=ax1)
    tidy = df_b.melt(id_vars='Kraj').rename(columns=str.title)
    # only change this line
    g = sns.lineplot(x='Kraj', y='Rate', data=df_b_rate, marker='s', color='red', ax=ax2)
    g.set(ylabel='Počet obyvatel na poskytovatele')

    g = sns.barplot(x='Kraj', y='Value', hue='Variable', data=tidy, ax=ax1)
    g.set(ylabel='Počet')

    g.legend(loc='center left', bbox_to_anchor=(-0.01, 1.125), labels=['Počet obyvatel', 'Počet poskytovatelů'])

    g.set(yscale='log')

    ax1.tick_params(axis='x', rotation=90)
    plt.show()
    save_file(graph_name, fig)


def query_b():
    inhabitants, providers, rate = prepare_dict_b()

    df_b = transform_dict_to_dataframe(inhabitants, ['Kraj', 'Sum_Inhabitants'])
    df_b_providers = transform_dict_to_dataframe(providers, ['Kraj', 'Sum_Providers'])
    df_b_rate = transform_dict_to_dataframe(rate, ['Kraj', 'Rate'])
    df_b['Sum_Providers'] = df_b_providers['Sum_Providers']

    plot_inhabitants_providers_rate(df_b, df_b_rate, 'pomer_obvytel_na_vseobecneho_lekare')

    df_b['Rate'] = df_b_rate['Rate']
    df_b.to_csv('DotazB.csv', sep=',', encoding='utf-8')


def prepare_custom_pharmacy_df():
    sort = {"$sort": {"Kraj": 1}}
    pipeline = [sort]
    results = list_of_collections[5].aggregate(pipeline)

    # Dict, ktorý obsahuje počet lekáreň v kraji
    region_pharmacy = {}
    for result in results:
        # Ak nie je kraj ako key v dicte, pridaj ho tam a vlož tam 1 (prvá lekáreň v kraji)
        if result["Kraj"] not in region_pharmacy and result["Kraj"] != '':
            region_pharmacy[result["Kraj"]] = 1
        elif result["Kraj"] != '':
            region_pharmacy[result["Kraj"]] += 1

    df_pharmacy = transform_dict_to_dataframe(region_pharmacy, ['Kraj', 'Sum'])
    return df_pharmacy


def plot_custom_pharmacy(df_pharmacy):
    fig = plt.figure(figsize=(10, 5))
    fig.tight_layout()
    fig.subplots_adjust(bottom=0.4)  # or whatever

    ax1 = fig.add_subplot(111)
    g = sns.barplot(x='Kraj', y='Sum', data=df_pharmacy, color='blue', ax=ax1)
    g.set(ylabel='Počet obyvatel na lékárnu')
    ax1.tick_params(axis='x', rotation=90)
    plt.show()
    save_file('lekarny', fig)


def query_custom_pharmacy():
    df_pharmacy = prepare_custom_pharmacy_df()
    df_pharmacy.to_csv('DotazCustomLekarny.csv', sep=',', encoding='utf-8')
    plot_custom_pharmacy(df_pharmacy)


def prepare_custom_gynecologist_df():
    sort = {"$sort": {"vuzemi_txt": 1}}
    match = {"$match": {"pohlavi_txt": "žena"}}
    pipeline = [sort, match]
    results = list_of_collections[3].aggregate(pipeline)
    regions_inhab_gyn = {}
    for result in results:
        # Ak nie je kraj ako key v dicte, pridaj ho tam a vlož tam 1 (prvá lekáreň v kraji)
        if result["vuzemi_txt"] not in regions_inhab_gyn:
            regions_inhab_gyn[result["vuzemi_txt"]] = 0 # when the result is not in dict set the value to 0 -> update on the next line
        regions_inhab_gyn[result["vuzemi_txt"]] += int(result["hodnota"])

    sort = {"$sort": {"Kraj": 1}}
    pipeline = [sort]
    results = list_of_collections[4].aggregate(pipeline)

    regions_providers_gyn = {}
    for result in results:
        # Ak nie je kraj ako key v dicte, pridaj ho tam a vlož tam 1 (prvá lekáreň v kraji)
        if result["Kraj"] not in regions_providers_gyn:
            regions_providers_gyn[result["Kraj"]] = 0 # when the result is not in dict set the value to 0 -> it will get updated on the next line
        regions_providers_gyn[result["Kraj"]] += 1


    # X = X prienik Y
    regions_inhab_gyn = {
        x: regions_inhab_gyn[x]
        for x in regions_inhab_gyn if x in regions_providers_gyn
    }
    # Y = Y prienik X
    regions_providers_gyn = {
        x: regions_providers_gyn[x]
        for x in regions_providers_gyn if x in regions_inhab_gyn
    }

    regions_rate = {}
    for region_provider in regions_providers_gyn:
        for region_inhabitant in regions_inhab_gyn:
            if region_inhabitant == region_provider:
                regions_rate[region_provider] = regions_inhab_gyn[region_inhabitant] / regions_providers_gyn[region_provider]
    return regions_inhab_gyn, regions_providers_gyn, regions_rate


def query_custom_gynecologist():
    inhabitants, providers, rate = prepare_custom_gynecologist_df()
    df_custom = transform_dict_to_dataframe(inhabitants, ['Kraj', 'Sum_Inhabitants'])
    df_custom_providers = transform_dict_to_dataframe(providers, ['Kraj', 'Sum_Providers'])
    df_custom_rate = transform_dict_to_dataframe(rate, ['Kraj', 'Rate'])
    df_custom['Sum_Providers'] = df_custom_providers['Sum_Providers']

    plot_inhabitants_providers_rate(df_custom, df_custom_rate, 'pomer_zen_na_gynekologa')

    df_custom['Rate'] = df_custom_rate['Rate']
    df_custom.to_csv('DotazCustomGynekologie.csv', sep=',', encoding='utf-8')


def query_c():
    health_care_types = [
        "dentální hygienistka", "dětské lékařství", "fyzioterapeut",
        "chirurgie", "kardiologie", "Optometrista",
        "ortopedie a traumatologie pohybového ústrojí",
        "praktické lékařství pro děti a dorost",
        "všeobecné praktické lékařství", "zubní lékařství"
    ]

    # Vytiahnem si dáta z kolekcie DotazA, ktoré sortnem podľa okresu
    sort = {"$sort": {"Okres": 1}}
    pipeline = [sort]
    results = list_of_collections[0].aggregate(pipeline)

    regions = {}
    for result in results:
        if result["Okres"] == "" or result["OborPece"] == "" or result["OborPece"] not in health_care_types:
            continue
        if result["Okres"] not in regions:
            regions[result["Okres"]] = Element(result["Okres"])
            regions[result["Okres"]].append_poskytovatel(result["OborPece"])
        else:
            regions[result["Okres"]].append_poskytovatel(result["OborPece"])

    # Vytiahnem si dáta z kolekcie DotazB_Obyvatelia, ktoré sortnem podľa okresu
    sort = {"$sort": {"vuzemi_txt": 1}}
    match = {"$match": {"vek_txt": {"$regex": "^(0|5|10)-(4|9|14)$"}}}
    pipeline = [sort, match]
    results = list_of_collections[1].aggregate(pipeline)

    for result in results:
        for key, value in regions.items():
            if key == result["vuzemi_txt"]:
                value.pocet_obyv_dorast = value.pocet_obyv_dorast + int(
                    result["hodnota"])

    # Vytiahnem si dáta z kolekcie DotazB_Obyvatelia, ktoré sortnem podľa okresu
    sort = {"$sort": {"vuzemi_txt": 1}}
    match = {
        "$match": {
            "vek_txt": {
                "$regex":
                    "^(20|25|30|35|40|45|50|55|60|)-(24|29|34|39|44|49|54|59|64)$"
            }
        }
    }
    pipeline = [sort, match]
    results = list_of_collections[1].aggregate(pipeline)

    for result in results:
        for key, value in regions.items():
            if key == result["vuzemi_txt"]:
                value.pocet_obyv_dospeli = value.pocet_obyv_dospeli + int(
                    result["hodnota"])

    # Vytiahnem si dáta z kolekcie DotazB_Obyvatelia, ktoré sortnem podľa okresu
    sort = {"$sort": {"vuzemi_txt": 1}}
    match = {"$match": {"vek_txt": {"$regex": "all"}}}
    pipeline = [sort, match]
    results = list_of_collections[1].aggregate(pipeline)

    for result in results:
        for key, value in regions.items():
            if key == result["vuzemi_txt"]:
                value.vsetci_obyvatelia = value.vsetci_obyvatelia + int(
                    result["hodnota"])
    df_c = pd.DataFrame(columns=('Okres', 'Počet obyvateľov dorast', 'Počet obyvateľov dospelí',
                                 'Počet všetkých obyvateľov', 'Percento dorast', 'Percento dospelí',
                                 'Odbor péče', 'Počet poskytovateľov'
                                 ))
    i = 0
    ### filling up the dataframe
    for key, value in regions.items():
        value.percento_dorast = (value.pocet_obyv_dorast /
                                 value.vsetci_obyvatelia) * 100
        value.percento_dospeli = (value.pocet_obyv_dospeli /
                                  value.vsetci_obyvatelia) * 100

        for provider in value.poskytovatelia:
            for key, val in provider.items():
                to_append = [value.okres, value.pocet_obyv_dorast, value.pocet_obyv_dospeli, value.vsetci_obyvatelia,
                             value.percento_dorast, value.percento_dospeli, key, val]
                df_c.loc[i] = to_append
                i += 1
    df_c_fixed = df_c[
        ['Okres', 'Počet všetkých obyvateľov', 'Počet obyvateľov dorast', 'Odbor péče', 'Počet poskytovateľov']]

    # column discretization
    df_c_fixed['Okres-kategoria'] = pd.cut(x=df_c_fixed['Počet všetkých obyvateľov'],
                                           bins=[0, 100000, 125000, 15000000], labels=['Malý', 'Stredný', 'Veľký'])

    # column normalization
    column = 'Počet všetkých obyvateľov'
    df_c_fixed['Počet všetkých obyvateľov (norm)'] = (df_c_fixed[column] - df_c_fixed[column].min()) / (
            df_c_fixed[column].max() - df_c_fixed[column].min())

    df_c.to_csv('DotazCPuvodni.csv', sep=',', encoding='utf-8')

    # finding outliers
    df_c_fixed = df_c_fixed.astype({'Počet poskytovateľov': float})
    z = np.abs(stats.zscore(df_c_fixed['Počet poskytovateľov']))
    threshold = 2

    outliers_position = np.where(z > threshold)
    median = df_c_fixed['Počet poskytovateľov'].median()

    ## drop rows with outliers
    for out_pos in outliers_position:
        for pos in out_pos:
            df_c_fixed.at[pos, 'Počet poskytovateľov'] = median

    df_c_fixed.to_csv('DotazCUpraveny.csv', sep=',', encoding='utf-8')


if __name__ == '__main__':
    list_of_collections = []
    list_of_collections = set_up_database()


    query_a()
    query_b()
    query_custom_pharmacy()
    query_custom_gynecologist()
    query_c()
