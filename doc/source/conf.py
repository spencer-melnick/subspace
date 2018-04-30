# -*- coding: utf-8 -*-

project = u'subspace'
copyright = u'2018, Spencer Melnick'
author = u'Spencer Melnick'
version = u'0.0'
release = u'0.0.0 Alpha'

extensions = ['breathe']
templates_path = ['templates']


breathe_projects = { "subspace": "../../build/doc/doxygen/xml" }
breathe_projects_source = {
    "subspace" : ( "../../src", ["main.cpp", "engine/config.hpp"])
}
breathe_default_project = "subspace"


source_suffix = '.rst'
master_doc = 'index'
language = None
exclude_patterns = []

pygments_style = 'sphinx'
highlight_language = 'c++'
primary_domain = 'cpp'


import sphinx_rtd_theme

html_theme = "sphinx_rtd_theme"
html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
html_logo = '../../rc/logo.png'
html_theme_options = {
    'logo_only': False,
    'display_version': False,
    'collapse_navigation': True,
}
html_static_path = ['static']
htmlhelp_basename = 'subspacedoc'


latex_elements = {
}

latex_documents = [
    (master_doc, 'subspace.tex', u'subspace Documentation',
     u'Spencer Melnick', 'manual'),
]


man_pages = [
    (master_doc, 'subspace', u'subspace Documentation',
     [author], 1)
]


texinfo_documents = [
    (master_doc, 'subspace', u'subspace Documentation',
     author, 'subspace', 'One line description of project.',
     'Miscellaneous'),
]