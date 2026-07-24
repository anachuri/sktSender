sktsender_sources = [
  'main.c',
  'sktsender-application.c',
  'sktsender-window.c',
]

sktsender_deps = [
  dependency('gtk4'),
]

sktsender_sources += gnome.compile_resources('sktsender-resources',
  'sktsender.gresource.xml',
  c_name: 'sktsender'
)

executable('sktsender', sktsender_sources,
  dependencies: sktsender_deps,
  export_dynamic:true,
       install: true,
)
