#!/usr/bin/env python3

import sys, re, os, argparse

'''
Skript zum Erzeugen einer tar-Datei mit den Loesungen von Uebungsaufgaben
'''

def course_patterns(possible_courses):
  return ', '.join(['/{}_'.format(key) for key in possible_courses])

def parse_arguments(possible_courses,names_file):
  s_courses_upper = ', '.join([c.upper() for c in possible_courses[:-1]])
  s_courses_upper += ' oder {}'.format(possible_courses[-1].upper())
  p = argparse.ArgumentParser(
          description=('Erzeuge .tar.gz file fuer die Abgabe der Loesungen '
                       'zu den Uebungen in {}. Die '
                       'Datei {} mit den Nachnamen der Gruppenmitglieder muss '
                       'im gleichen Verzeichnis '
                       'stehen, wie die Verzeichnisse Blatt01.*, Blatt02.* etc '
                       'mit Ihren Loesungen. Jeder Nachname muss mit einem '
                       'Grossbuchstaben beginnen und darf sonst nur '
                       'Klein- und Grossbuchstaben, Bindestriche und '
                       'Unterstriche enthalten. Die Liste der Nachname, die '
                       'zeilenweise angegeben wird, muss lexikographisch '
                       'sortiert sein. Im Pfad, in dem '
                       'dieses Skript ausgefuehrt wird, muss genau einer der '
                       'Strings {} vorkommen.')
                       .format(s_courses_upper,names_file,
                               course_patterns(possible_courses)))
  p.add_argument('blattnummer',type=int,default=None,metavar='<int>',
                  help='Angabe der Nummer des Blattes')
  return p.parse_args()

def course_get(possible_courses):
  current_dir = os.getcwd()
  for key in possible_courses:
    m = re.search(r'/{}_\d{{4}}'.format(key),current_dir)
    if m:
      return key
  s_keys = ', '.join(['{}_'.format(key) for key in possible_courses])
  sys.stderr.write(('{}: im Pfad {} fehlt einer der Schluessel {} zur '
                    'Identifikation des Moduls\n')
                    .format(sys.argv[0],current_dir,s_keys))
  exit(1)

def names_get(names_file):
  try:
    stream = open(names_file)
  except IOError:
    sys.stderr.write(('{}: eine Datei mit dem Namen {} konnte nicht geoeffnet '
                      'werden\n').format(sys.argv[0],names_file))
    exit(1)
  names = list()
  for line in stream:
    name = line.rstrip()
    if not re.search(r'^[A-Za-z][A-Za-z\-_]+$',name):
      sys.stderr.write(('{}: der Name {} in der Datei enthaelt nicht erlaubte '
                        'Zeichen.\n')
                        .format(sys.argv[0],name))
      exit(1)
    names.append(name)
  stream.close()
  for pname, name in zip(names,names[1:]):
    if pname > name:
      sys.stderr.write(('{}: die Liste der Nachnamen der Gruppenmitglieder '
                        'in der Datei {} muss lexikographisch sortiert sein. '
                        'Die Namen {} und {} '
                        'in dieser Reihenfolge widersprechen dieser Regel\n')
                        .format(sys.argv[0],names_file,pname,name))
      exit(1)
  return names

possible_courses = ['pfn1','pfn2','gsa','gik']
names_file = 'namen.txt'
args = parse_arguments(possible_courses,names_file)
names = names_get(names_file)
prefix = 'Blatt{:02d}.{}'.format(args.blattnummer,'.'.join(names))
if not os.path.isdir(prefix):
  sys.stderr.write(('{}: das erwartete Verzeichnis {} mit Ihren Loesungen '
                    'zu Blatt {} der {}-Uebung existiert nicht\n')
                    .format(sys.argv[0],prefix,args.blattnummer,
                            course_get(possible_courses)))
  sys.stderr.write('sie muessen ggf. noch den folgenden Befehl ausfuehren\n')
  sys.stderr.write('mv Blatt{:02d} {}\n'.format(args.blattnummer,prefix))
  exit(1)
os.system('tar -cvzf {}.tar.gz {}'.format(prefix,prefix))
sys.stderr.write('# tar file {}.tar.gz wurde erzeugt\n'.format(prefix))
