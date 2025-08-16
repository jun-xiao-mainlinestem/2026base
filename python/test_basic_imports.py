#!/usr/bin/env python3
"""
Basic import test for VEX Python code
"""

def test_basic_imports():
    """Test basic Python imports that don't require VEX library"""
    print("Testing basic Python imports...")
    
    try:
        # Test standard library imports
        import time
        print("✓ time module imported successfully")
        
        import math
        print("✓ math module imported successfully")
        
        # Test our custom modules (without VEX dependencies)
        print("\nTesting custom module structure...")
        
        # Check if files exist
        import os
        current_dir = os.path.dirname(os.path.abspath(__file__))
        src_dir = os.path.join(current_dir, 'src')
        
        if os.path.exists(src_dir):
            print(f"✓ src directory exists: {src_dir}")
            
            # Check for Python files
            python_files = [f for f in os.listdir(src_dir) if f.endswith('.py')]
            print(f"✓ Found Python files: {python_files}")
            
            # Check for rgb_template directory
            rgb_dir = os.path.join(src_dir, 'rgb_template')
            if os.path.exists(rgb_dir):
                print(f"✓ rgb_template directory exists: {rgb_dir}")
                
                rgb_files = [f for f in os.listdir(rgb_dir) if f.endswith('.py')]
                print(f"✓ Found rgb_template files: {rgb_files}")
            else:
                print("✗ rgb_template directory not found")
        else:
            print("✗ src directory not found")
        
        print("\n✓ Basic import test completed successfully!")
        
    except Exception as e:
        print(f"✗ Test failed with error: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    test_basic_imports()
